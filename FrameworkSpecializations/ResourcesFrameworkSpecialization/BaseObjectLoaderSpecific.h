#ifndef OBJECTLOADERS_SPECIFIC_H_
#define OBJECTLOADERS_SPECIFIC_H_

#include "Framework/ResourcesFramework/BaseObjectLoader.h"

//Specific objects
#include "GraphicFramework/Graphic/Texture.h"
#include "GraphicFramework/Graphic/Material.h"
#include "GraphicFramework/Graphic/Shader.h"
#include "GraphicFramework/Graphic/ShaderProgram.h"
#include "GraphicFramework/Resources/SystemInfo.h"
#include "GraphicFramework/Resources/ModelFileDescription.h"
#include "GraphicFramework/Resources/MeshHolder.h"

namespace Resources {

#ifndef CONCRETE_CPP
extern template class BaseObjectLoader<Resources::MeshHolder, root_files_path>;
extern template class BaseObjectLoader<Graphic::Texture, texture_files_path>;
extern template class BaseObjectLoader<Graphic::Material, obj_files_path>;
extern template class BaseObjectLoader<Graphic::Shader, shader_files_path>;
extern template class BaseObjectLoader<Graphic::ShaderProgram, shader_program_files_path>;
extern template class BaseObjectLoader<Resources::SystemInfo>;
extern template class BaseObjectLoader<Resources::ModelFileDescription, model_description_path>;
#endif

//Vertex
typedef BaseObjectLoader<Resources::MeshHolder, root_files_path>  MeshObjectLoader;
typedef BaseObjectLoader<Graphic::Texture, texture_files_path> TexturesObjectLoader;
typedef BaseObjectLoader<Graphic::Material, obj_files_path> MaterialObjectLoader;
typedef BaseObjectLoader<Graphic::Shader, shader_files_path> ShaderObjectLoader;
typedef BaseObjectLoader<Graphic::ShaderProgram, shader_program_files_path> ShaderProgramObjectLoader;
typedef BaseObjectLoader<Resources::SystemInfo> SystemInfoObjectLoader;
typedef BaseObjectLoader<Resources::ModelFileDescription, model_description_path> ModelFileDescriptionObjectLoader;
} /* namespace Resources */

#endif /* OBJECTLOADERS_SPECIFIC_H_ */
