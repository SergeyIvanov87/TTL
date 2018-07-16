#ifndef RESOURCESPATH_H
#define RESOURCESPATH_H

extern const char main_resources_path[];
extern const char root_files_path[];
extern const char obj_files_path[];
extern const char texture_files_path[];
extern const char skelobj_files_path[];
extern const char shader_files_path[];
extern const char shader_program_files_path[];
extern const char system_info_dummy_path[];
extern const char model_description_path[];
extern const char tmpDirectory[];

namespace Resources
{
    enum ResourceType
    {
        EMPTY = -1,//0x0,
        SYSTEM_INFO,
        MODEL_DESCRIPTION,
        SHADER,
        SHADER_PROGRAM,
        STATIC_VERTICES,
        SKELETON_VERTICES = STATIC_VERTICES,
        MATERIAL = STATIC_VERTICES + 1,
        TEXTURE,
        SOUND,
        ANIMATION,
        ANIMATION_CONTAINER_STORAGE,
        MAX_RESOURCE_TYPE
    };

inline const char *getResourceTypeString(ResourceType type)
{
    static const char *strings[] = {"SYSTEM_INFO", "MODEL_DESCRIPTION",
                                    "SHADER", "SHADER_PROGRAM",
                                    "VERTICES", "MATERIAL", "TEXTURE",
                                    "SOUND", "ANIMATION", "ANIMATION_CONTAINER_STORAGE"};
    static_assert(sizeof(strings)/sizeof(strings[0]) == MAX_RESOURCE_TYPE,
            "Invalid ResourceType and ResourceType description count");
    if(type == EMPTY)
    {
        return "INVALID RESOURCE TYPE";
    }
    return type < MAX_RESOURCE_TYPE ? strings[type] : "INVALID RESOURCE TYPE";
}
}
#endif //RESOURCESPATH_H
