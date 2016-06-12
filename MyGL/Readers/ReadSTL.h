#ifndef GRAPHIC_LIBRARY_READSTL_H
#define GRAPHIC_LIBRARY_READSTL_H

namespace MyGL {

    class Scene;

    namespace Readers {

        class ReadSTL {
        public:
            static MyGL::Scene read(const char *fileName);

        private:
            static MyGL::Scene readASCII(const char *fileName);
            static MyGL::Scene readBinary(const char *fileName);
        };

    }

}

#endif // GRAPHIC_LIBRARY_READSTL_H
