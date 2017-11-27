#pragma once
#include <string>
#include <vector>
#include <tinyxml2.h>
namespace t800 {
  struct T8TechniqueProfile;
  class T8Technique {
  private:
    void ProcessDefine(tinyxml2::XMLElement* element);
    void ProcessProfile(tinyxml2::XMLElement* element);
    void ProcessShader(tinyxml2::XMLElement* element);
    std::vector<std::string>* m_actualDefines;
  public:
    T8Technique();
    explicit T8Technique(std::string path);
    void Parse(std::string path);

    enum T8_TECHNIQUE_PROFILE {
      HLSL,
      GLES20,
      GLES30,
      GL
    };
    std::string m_name;
    std::vector<T8TechniqueProfile> m_profiles;

    std::vector<std::string> m_globalDefines;
    tinyxml2::XMLDocument m_xmlDoc;
  };
  struct T8TechniqueProfile {
    T8Technique::T8_TECHNIQUE_PROFILE m_type;
    std::string m_name;

    std::string m_vsPath;
    std::string m_fsPath;
    std::string m_gsPath;
    std::string m_hsPath;
    std::string m_tsPath;

    std::vector<std::string> m_defines;
  };
}