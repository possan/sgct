/*************************************************************************
Copyright (c) 2012-2015 Miroslav Andel
All rights reserved.

For conditions of distribution and use, see copyright notice in sgct.h 
*************************************************************************/

#ifndef __SGCT__SHADER_DATA__H__
#define __SGCT__SHADER_DATA__H__

#include <sgct/shader.h>

namespace sgct::core {

struct ShaderData {
    Shader shader;
    std::string source;
    bool isSrcFile;
};

} // namespace sgct::core

#endif // __SGCT__SHADER_DATA__H__
