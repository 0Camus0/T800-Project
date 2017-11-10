#ifndef T800_DESCRIPTORS_H
#define T800_DESCRIPTORS_H

namespace T8_BUFFER_USAGE{
  enum E {
    DEFAULT,
    DINAMIC,
    STATIC
  };
}

namespace T8_IB_FORMAR {
  enum E {
    R16,
    R32
  };
}

namespace T8_TOPOLOGY {
  enum E {
    POINT_LIST,
    LINE_LIST,
    LINE_STRIP,
    TRIANLE_LIST,
    TRIANGLE_STRIP
  };
}

namespace T8_SHADER_TYPE {
  enum E {
    VERTEX,
    FRAGMENT
  };
}

namespace T8_BUFFER_TYPE {
  enum E {
    VERTEX,
    INDEX,
    CONSTANT
  };
}


namespace t800 {
  struct BufferDesc {
    int byteWidth;
    T8_BUFFER_USAGE::E usage;
  };
}


#endif
