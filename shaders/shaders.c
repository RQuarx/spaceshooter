#include "shaders.h"


const uint8_t shader_fragment[] = {
#embed "fragment.spv"
};
const size_t shader_fragment_size = sizeof shader_fragment;


const uint8_t shader_vertex[] = {
#embed "vertex.spv"
};
const size_t shader_vertex_size = sizeof shader_vertex;
