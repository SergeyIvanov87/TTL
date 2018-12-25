/*
 * VertexObjectLoader.h
 *
 *  Created on: Feb 11, 2016
 *      Author: user
 */
#if 0
#ifndef VERTEXOBJECTLOADER_H_
#define VERTEXOBJECTLOADER_H_
#include "../Framework/Base/Resources/ResourcesPath.h"
#include <Framework/ResourcesFramework/BaseObjectLoader.h>
#include "../Graphic/Vertices.h"
#include "../Graphic/SkeletonVertices.h"
#include "../Framework/Base/Graphic/Texture.h"
#include "../Framework/Base/Graphic/Material.h"
#include "../Framework/Base/Graphic/Shader.h"
#include "../Framework/Base/Graphic/ShaderProgram.h"
#include "../Framework/Base/Resources/SystemInfo.h"
#include "../Framework/Base/Resources/ModelFileDescription.h"

#include "../Framework/Base/Resources/MeshHolder.h"

namespace Resources {

/*
typedef BaseObjectLoader<Graphic::Vertices, obj_files_path> VertexObjectLoader;
typedef BaseObjectLoader<Graphic::SkeletonVertices, skelobj_files_path> SkeletonVertexObjectLoader;
*/
#ifndef CONCRETE_CPP
extern template class BaseObjectLoader<MeshHolder, root_files_path>;
extern template class BaseObjectLoader<Graphic::Texture, texture_files_path>;
extern template class BaseObjectLoader<Graphic::Material, obj_files_path>;
extern template class BaseObjectLoader<Graphic::Shader, shader_files_path>;
extern template class BaseObjectLoader<Graphic::ShaderProgram, shader_program_files_path>;
extern template class BaseObjectLoader<SystemInfo>;
extern template class BaseObjectLoader<ModelFileDescription, model_description_path>;
#endif

//Vertex
typedef BaseObjectLoader<MeshHolder, root_files_path>  MeshObjectLoader;
typedef BaseObjectLoader<Graphic::Texture, texture_files_path> TexturesObjectLoader;
typedef BaseObjectLoader<Graphic::Material, obj_files_path> MaterialObjectLoader;
typedef BaseObjectLoader<Graphic::Shader, shader_files_path> ShaderObjectLoader;
typedef BaseObjectLoader<Graphic::ShaderProgram, shader_program_files_path> ShaderProgramObjectLoader;
typedef BaseObjectLoader<SystemInfo> SystemInfoObjectLoader;
typedef BaseObjectLoader<ModelFileDescription, model_description_path> ModelFileDescriptionObjectLoader;
} /* namespace Resources */

#endif /* VERTEXOBJECTLOADER_H_ */
#endif
