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

namespace t800 {
  struct BufferDesc {
    int byteWidth;
    T8_BUFFER_USAGE::E usage;
  };
}


#endif
