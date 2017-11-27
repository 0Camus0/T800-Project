#include "utils\T8_Technique.h"
#include <iostream>
namespace t800 {
  void T8Technique::ProcessDefine(tinyxml2::XMLElement * element)
  {
    const char* name = element->Attribute("name");
    if (name == nullptr) {
      std::cout << "error: atribute name not found on define" << std::endl;
      m_xmlDoc.Clear();
      return;
    }
    else {
      std::string def;
      def = "#define ";
      def += name;
      const char* value = element->Attribute("value");
      if (value != nullptr) {
        def += " ";
        def += value;
      }
      m_actualDefines->push_back(def);
    }
    
  }

  void T8Technique::ProcessProfile(tinyxml2::XMLElement * element)
  {
    auto child = element->FirstChildElement("define");
    if (child != nullptr) {
      do {
        m_actualDefines = &m_profiles.back().m_defines;
        ProcessDefine(child);
        child = child->NextSiblingElement("define");
      } while (child != nullptr);
    }
    auto shader = element->FirstChildElement("shader");
    if (shader == nullptr) {
      std::cout << "error: shader not found on profile" << std::endl;
      m_xmlDoc.Clear();
      return;
    }
    else {
      do {
        ProcessShader(shader);
        shader = shader->NextSiblingElement("shader");
      } while (shader != nullptr);
    }
  }

  void T8Technique::ProcessShader(tinyxml2::XMLElement * element)
  {
    const char* path = element->Attribute("path");
    if (path == nullptr) {
      std::cout << "error: atribute path not found on shader" << std::endl;
      m_xmlDoc.Clear();
      return;
    }
    else {
      const char* type = element->Attribute("type");
      if (type == nullptr) {
        std::cout << "error: atribute type not found on shader" << std::endl;
        m_xmlDoc.Clear();
        return;
      }
      else {
        if (std::string(type) == "VS")
          m_profiles.back().m_vsPath = path;
        else if (std::string(type) == "FS")
          m_profiles.back().m_fsPath = path;
      }
    }
  }


  T8Technique::T8Technique()
  {
  }
  T8Technique::T8Technique(std::string path)
  {
    Parse(path);
  }
  void T8Technique::Parse(std::string path)
  {
    m_actualDefines = &m_globalDefines;
    m_xmlDoc.LoadFile(path.c_str());
    auto technique = m_xmlDoc.FirstChildElement("technique");
    const char* att = technique->Attribute("name");
    if (att == nullptr)
    {
      std::cout << "error: atribute name not found on technique" << std::endl;
      m_xmlDoc.Clear();
      return;
    }
    m_name = att;
    auto global = technique->FirstChildElement("global");
    if (global == nullptr) {
      std::cout << "warning: global label not found" << std::endl;
    }
    else {
      auto child = global->FirstChildElement("define");
      if (child == nullptr) {
        std::cout << "warning: no defines on global scope" << std::endl;
      }
      else {
        do {
          ProcessDefine(child);
          child = child->NextSiblingElement("define");
        } while (child != nullptr);
      }
    }
    //Profiles
    auto profile = technique->FirstChildElement("profile");
    if (profile == nullptr) {
      std::cout << "error: no profile found" << std::endl;
      m_xmlDoc.Clear();
      return;
    }
    else {
      do {
        T8TechniqueProfile t8Prof;
        t8Prof.m_name = profile->Attribute("name");
        std::string type = profile->Attribute("type");
        if (type == "GLSL")
          t8Prof.m_type = T8_TECHNIQUE_PROFILE::GL;
        else if (type == "HLSL")
          t8Prof.m_type = T8_TECHNIQUE_PROFILE::HLSL;
        else if (type == "GLSL_ES2")
          t8Prof.m_type = T8_TECHNIQUE_PROFILE::GLES20;
        else if (type == "GLSL_ES3")
          t8Prof.m_type = T8_TECHNIQUE_PROFILE::GLES30;
        else {
          std::cout << "error: profile type not specified" << std::endl;
          m_xmlDoc.Clear();
          return;
        }

        m_profiles.push_back(t8Prof);
        ProcessProfile(profile);
        profile = profile->NextSiblingElement("profile");
      } while (profile != nullptr);
    }
    m_xmlDoc.Clear();
  }
}