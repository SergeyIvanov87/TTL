#define CONCRETE_CPP
#include "BaseObjectLoaderSPecific.h"
#include "../Framework/ResourcesFramework/BaseObjectLoader.hpp"
#undef CONCRETE_CPP

namespace Resources
{
template class BaseObjectLoader<Resources::MeshHolder, root_files_path>;
template class BaseObjectLoader<Graphic::Texture, texture_files_path>;
template class BaseObjectLoader<Graphic::Material, obj_files_path>;
template class BaseObjectLoader<Graphic::Shader, shader_files_path>;
template class BaseObjectLoader<Graphic::ShaderProgram, shader_program_files_path>;
template class BaseObjectLoader<Resources::SystemInfo>;
template class BaseObjectLoader<Resources::ModelFileDescription, model_description_path>;
}
