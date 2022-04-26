#include <sstream>
#include <fstream>
#include <random>
#include <chrono>

#include <nlohmann/json.hpp>

#include "botcraft/Game/World/Blockstate.hpp"
#include "botcraft/Utilities/Logger.hpp"
#include "botcraft/Utilities/StringUtilities.hpp"

#if USE_GUI
#include "botcraft/Renderer/Atlas.hpp"
#endif

namespace Botcraft
{
    // Utilities functions

    Model ModelModificationFromJson(const Model &model, const nlohmann::json &json)
    {
        Model output = model;

        bool uv_lock = false;
        if (json.contains("uvlock"))
        {
            uv_lock = json["uvlock"];
        }

        int rotation_x = 0;
        if (json.contains("x"))
        {
            rotation_x = json["x"];
        }

        int rotation_y = 0;
        if (json.contains("y"))
        {
            rotation_y = json["y"];
        }

        //Rotate colliders

        if (rotation_x != 0)
        {
            std::vector<AABB> &colliders = output.GetColliders();

            for (int i = 0; i < colliders.size(); ++i)
            {
                Vector3<double> new_center;
                switch (rotation_x / 90)
                {
                case 0:
                    break;
                case 1:
                    new_center = colliders[i].GetCenter() - 0.5;
                    new_center = Vector3<double>(new_center.x, new_center.z, -new_center.y);
                    new_center = new_center + 0.5;
                    colliders[i] = AABB(new_center, Vector3<double>(colliders[i].GetHalfSize().x, colliders[i].GetHalfSize().z, colliders[i].GetHalfSize().y));
                    break;
                case 2:
                    new_center = colliders[i].GetCenter() - 0.5;
                    new_center = Vector3<double>(new_center.x, -new_center.y, -new_center.z);
                    new_center = new_center + 0.5;
                    colliders[i] = AABB(new_center, Vector3<double>(colliders[i].GetHalfSize().x, colliders[i].GetHalfSize().y, colliders[i].GetHalfSize().z));
                    break;
                case 3:
                    new_center = colliders[i].GetCenter() - 0.5;
                    new_center = Vector3<double>(new_center.x, -new_center.z, new_center.y);
                    new_center = new_center + 0.5;
                    colliders[i] = AABB(new_center, Vector3<double>(colliders[i].GetHalfSize().x, colliders[i].GetHalfSize().z, colliders[i].GetHalfSize().y));
                    break;
                default:
                    LOG_ERROR("Blockstate X rotation should be in 90� steps");
                    break;
                }
            }
        }

        if (rotation_y != 0)
        {
            std::vector<AABB> &colliders = output.GetColliders();

            for (int i = 0; i < colliders.size(); ++i)
            {
                Vector3<double> new_center;
                switch (rotation_y / 90)
                {
                case 0:
                    break;
                case 1:
                    new_center = colliders[i].GetCenter() - 0.5;
                    new_center = Vector3<double>(-new_center.z, new_center.y, new_center.x);
                    new_center = new_center + 0.5;
                    colliders[i] = AABB(new_center, Vector3<double>(colliders[i].GetHalfSize().z, colliders[i].GetHalfSize().y, colliders[i].GetHalfSize().x));
                    break;
                case 2:
                    new_center = colliders[i].GetCenter() - 0.5;
                    new_center = Vector3<double>(-new_center.x, new_center.y, -new_center.z);
                    new_center = new_center + 0.5;
                    colliders[i] = AABB(new_center, Vector3<double>(colliders[i].GetHalfSize().x, colliders[i].GetHalfSize().y, colliders[i].GetHalfSize().z));
                    break;
                case 3:
                    new_center = colliders[i].GetCenter() - 0.5;
                    new_center = Vector3<double>(new_center.z, new_center.y, -new_center.x);
                    new_center = new_center + 0.5;
                    colliders[i] = AABB(new_center, Vector3<double>(colliders[i].GetHalfSize().z, colliders[i].GetHalfSize().y, colliders[i].GetHalfSize().x));
                    break;
                default:
                    LOG_ERROR("Blockstate Y rotation should be in 90� steps");
                    break;
                }
            }
        }

#ifdef USE_GUI
        //Rotate faces

        if (rotation_x != 0)
        {
            std::vector<FaceDescriptor> &faces = output.GetFaces();

            std::vector<Orientation> rotated_orientations = { Orientation::Top, Orientation::North, Orientation::Bottom, Orientation::South };

            for (int f = 0; f < faces.size(); ++f)
            {
                //We want to add this transformation after the local translations,
                //Just before the global translation to final position
                faces[f].transformations.translations.insert(faces[f].transformations.translations.begin(), Renderer::TransformationPtr(new Renderer::Rotation(1.0f, 0.0f, 0.0f, -rotation_x)));

                const auto it_cullface = std::find(rotated_orientations.begin(), rotated_orientations.end(), faces[f].cullface_direction);

                if (it_cullface != rotated_orientations.end())
                {
                    faces[f].cullface_direction = rotated_orientations[((it_cullface - rotated_orientations.begin()) + rotation_x / 90) % rotated_orientations.size()];
                }

                if (uv_lock)
                {
                    const auto it_orientation = std::find(rotated_orientations.begin(), rotated_orientations.end(), faces[f].orientation);
                    if (it_orientation == rotated_orientations.end())
                    {
                        faces[f].transformations.rotation -= rotation_x / 90;
                    }
                }
                faces[f].face = Renderer::Face(faces[f].transformations, faces[f].orientation);
            }
        }

        if (rotation_y != 0)
        {
            std::vector<FaceDescriptor> &faces = output.GetFaces();

            std::vector<Orientation> rotated_orientations = { Orientation::South, Orientation::West, Orientation::North, Orientation::East };

            for (int f = 0; f < faces.size(); ++f)
            {
                //We want to add this transformation after the local translations,
                //Just before the global translation to final position
                faces[f].transformations.translations.insert(faces[f].transformations.translations.begin(), Renderer::TransformationPtr(new Renderer::Rotation(0.0f, 1.0f, 0.0f, -rotation_y)));

                const auto it_cullface = std::find(rotated_orientations.begin(), rotated_orientations.end(), faces[f].cullface_direction);

                if (it_cullface != rotated_orientations.end())
                {
                    faces[f].cullface_direction = rotated_orientations[((it_cullface - rotated_orientations.begin()) + rotation_y / 90) % rotated_orientations.size()];
                }

                if (uv_lock)
                {
                    const auto it_orientation = std::find(rotated_orientations.begin(), rotated_orientations.end(), faces[f].orientation);
                    if (it_orientation == rotated_orientations.end())
                    {
                        // Not sure why it's always 2 here...
                        faces[f].transformations.rotation += 2;
                    }
                }
                faces[f].face = Renderer::Face(faces[f].transformations, faces[f].orientation);
            }
        }
#endif

        return output;
    }

    std::string ModelNameFromJson(const nlohmann::json &json)
    {
        std::string model_path = json["model"];
#if PROTOCOL_VERSION < 347
        return "block/" + model_path;
#elif PROTOCOL_VERSION > 578 //> 1.15.2
        // Remove the minecraft: prefix from the model path
        return model_path.substr(10);
#else
        return model_path;
#endif
    }

    int WeightFromJson(const nlohmann::json &json)
    {
        int output = 1;

        if (json.contains("weight"))
        {
            output = json["weight"];
        }

        return output;
    }

    const bool CheckCondition(const std::string &name, const std::string &value, const std::vector<std::string> &variables)
    {
        const std::vector<std::string> possible_values = SplitString(value, '|');

        bool output = false;

        for (int i = 0; i < variables.size(); ++i)
        {
            if (StartsWith(variables[i], name))
            {
                for (int j = 0; j < possible_values.size(); ++j)
                {
                    if (EndsWith(variables[i], possible_values[j]))
                    {
                        output = true;
                        break;
                    }
                }
                if (output)
                {
                    break;
                }
            }
        }

        return output;
    }

    // Blockstate implementation starts here
    std::map<std::string, nlohmann::json> Blockstate::cached_jsons;

#if PROTOCOL_VERSION < 347
    Blockstate::Blockstate(const int id_, const unsigned char metadata_, 
                           const bool transparent_, const bool solid_, const bool fluid_, const bool custom,
                           const float hardness_, const TintType tint_type_, const std::string &name_,
                           const std::string &path, const std::vector<std::string> &variables_) :
                           id(id_), metadata(metadata_), transparent(transparent_), solid(solid_), fluid(fluid_),
                           hardness(hardness_), tint_type(tint_type_), m_name(name_)
#else
    Blockstate::Blockstate(const int id_,
                           const bool transparent_, const bool solid_, const bool fluid_, const bool custom,
                           const float hardness_, const TintType tint_type_, const std::string &name_,
                           const std::string &path, const std::vector<std::string> &variables_) :
                           id(id_), transparent(transparent_), solid(solid_), fluid(fluid_),
                           hardness(hardness_), tint_type(tint_type_), m_name(name_)
#endif
    {
        weights_sum = 0;
//        random_generator = std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count());

        if (path == "none")
        {
            models.push_back(Model());
            models_weights.push_back(1);
            weights_sum += 1;
            return;
        }

        if (path.empty())
        {
            models.push_back(Model::GetModel("", false));
            models_weights.push_back(1);
            weights_sum += 1;

            return;
        }

        std::string full_filepath;
        
        if (custom)
        {
            full_filepath = ASSETS_PATH + std::string("/custom/blockstates/") + path + ".json";
        }
        else
        {
            full_filepath = ASSETS_PATH + std::string("/minecraft/blockstates/") + path + ".json";
        }

        try
        {
            auto it = cached_jsons.find(full_filepath);
            if (it == cached_jsons.end())
            {
                std::ifstream file(full_filepath);
                file >> cached_jsons[full_filepath];
                file.close();
            }
        }
        catch (const nlohmann::json::exception& e)
        {
            if (custom)
            {
                LOG_ERROR("Missing custom definition for " << full_filepath << '\n' << e.what());
            }
            else
            {
                LOG_ERROR("Error reading blockstate file at " << full_filepath << '\n' << e.what());
            }
            
            models.push_back(Model::GetModel("", false));
            models_weights.push_back(1);
            weights_sum += 1;

            return;
        }

        const nlohmann::json& json = cached_jsons[full_filepath];

        // We store the models in a deque for efficiency
        std::deque<Model> models_deque;

        for (int i = 0; i < variables_.size(); ++i)
        {
            std::vector<std::string> splitted = SplitString(variables_[i], '=');
            variables[splitted[0]] = splitted[1];
        }

        //If it's a "normal" blockstate
        if (json.contains("variants"))
        {
            nlohmann::json null_value;
            nlohmann::json &variant_value = null_value;

            if (json["variants"].contains(""))
            {
                variant_value = json["variants"][""];
            }

            if (json["variants"].contains("normal"))
            {
                variant_value = json["variants"]["normal"];
            }

            //This case means we have to check the variables to find
            //the right variant
            if (variables_.size() > 0 && variant_value.is_null())
            {
                int max_match = 0;

                for (nlohmann::json::const_iterator it = json["variants"].begin(); it != json["variants"].end(); ++it)
                {
                    const std::vector<std::string> variables_values = SplitString(it.key(), ',');
                    
                    int num_match = 0;
                    for (int i = 0; i < variables_.size(); ++i)
                    {
                        for (int j = 0; j < variables_values.size(); ++j)
                        {
                            if (variables_[i] == variables_values[j])
                            {
                                num_match++;
                            }
                        }
                    }
                    if (num_match > max_match)
                    {
                        variant_value = it.value();
                        max_match = num_match;
                    }
                }
            }

            //If we have found a correct value
            if (!variant_value.is_null())
            {
                if (variant_value.is_array())
                {
                    for (auto& model : variant_value)
                    {
                        const int weight = WeightFromJson(model);
                        models_deque.push_back(ModelModificationFromJson(Model::GetModel(ModelNameFromJson(model), custom), model));
                        models_weights.push_back(weight);
                        weights_sum += weight;
                    }
                }
                else
                {
                    const int weight = WeightFromJson(variant_value);
                    models_deque.push_back(ModelModificationFromJson(Model::GetModel(ModelNameFromJson(variant_value), custom), variant_value));
                    models_weights.push_back(weight);
                    weights_sum += weight;
                }
            }
            else
            {
                LOG_ERROR("Error reading " << full_filepath);
                models_deque.push_back(Model::GetModel("", false));
                models_weights.push_back(1);
                weights_sum += 1;
            }
        }

        //If it's a multipart blockstate
        if (json.contains("multipart"))
        {
            //Start with an empty model
            models_deque.push_back(Model());
            models_weights.push_back(1);
            weights_sum = 0;

            for (auto& part : json["multipart"])
            {
                //If no condition
                if (!part.contains("when"))
                {
                    //If there are several models
                    if (part["apply"].is_array())
                    {
                        size_t num_models = models_deque.size();
                        for (auto& m : part["apply"])
                        {
                            const std::string model_name = ModelNameFromJson(m);
                            for (int k = 0; k < num_models; ++k)
                            {
                                models_deque.push_back(models_deque[k] + ModelModificationFromJson(Model::GetModel(model_name, custom), m));
                                models_weights.push_back(models_weights[k] * WeightFromJson(m));
                            }
                        }
                        models_deque.erase(models_deque.begin(), models_deque.begin() + num_models);
                        models_weights.erase(models_weights.begin(), models_weights.begin() + num_models);
                    }
                    else
                    {
                        const std::string model_name = ModelNameFromJson(part["apply"]);
                        for (int k = 0; k < models_deque.size(); ++k)
                        {
                            models_deque[k] += ModelModificationFromJson(Model::GetModel(model_name, custom), part["apply"]);
                            models_weights[k] *= WeightFromJson(part["apply"]);
                        }
                    }
                }
                //There is a condition !
                //We check if there is a match with the variables
                else
                {
                    bool condition = false;
                    //If it's a OR condition
                    if (part["when"].contains("OR"))
                    {
                        for (auto& current_condition : part["when"]["OR"])
                        {
                            for (nlohmann::json::const_iterator condition_it = current_condition.begin(); condition_it != current_condition.end(); ++condition_it)
                            {
                                const std::string condition_name = condition_it.key();
                                std::string condition_value = "";

                                if (condition_it.value().is_string())
                                {
                                    condition_value = condition_it.value();
                                }
                                else if (condition_it.value().is_boolean())
                                {
                                    condition_value = condition_it.value().get<bool>() ? "true" : "false";
                                }
                                else if (condition_it.value().is_number())
                                {
                                    condition_value = std::to_string(condition_it.value().get<double>());
                                }

                                condition = CheckCondition(condition_name, condition_value, variables_);
                                //If one condition in the list is not verified,
                                //the whole condition is not
                                if (!condition)
                                {
                                    break;
                                }
                            }

                            if (condition)
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        for (nlohmann::json::const_iterator condition_it = part["when"].begin(); condition_it != part["when"].end(); ++condition_it)
                        {
                            std::string condition_value = "";

                            if (condition_it.value().is_string())
                            {
                                condition_value = condition_it.value();
                            }
                            else if (condition_it.value().is_boolean())
                            {
                                condition_value = condition_it.value().get<bool>() ? "true" : "false";
                            }
                            else if (condition_it.value().is_number())
                            {
                                condition_value = std::to_string(condition_it.value().get<double>());
                            }

                            condition = CheckCondition(condition_it.key(), condition_value, variables_);
                            //If one condition in the list is not verified,
                            //the whole condition is not
                            if (!condition)
                            {
                                break;
                            }
                        }
                    }

                    //If the condition is verified, add the model
                    if (condition)
                    {
                        //If there are several models
                        if (part["apply"].is_array())
                        {
                            size_t num_models = models_deque.size();
                            for (auto& m : part["apply"])
                            {
                                const std::string model_name = ModelNameFromJson(m);
                                const int model_weight = WeightFromJson(m);
                                for (int k = 0; k < num_models; ++k)
                                {
                                    models_deque.push_back(models_deque[k] + ModelModificationFromJson(Model::GetModel(model_name, custom), m));
                                    models_weights.push_back(models_weights[k] * model_weight);
                                }
                            }
                            models_deque.erase(models_deque.begin(), models_deque.begin() + num_models);
                            models_weights.erase(models_weights.begin(), models_weights.begin() + num_models);
                        }
                        else
                        {
                            const std::string model_name = ModelNameFromJson(part["apply"]);
                            const int model_weight = WeightFromJson(part["apply"]);
                            for (int k = 0; k < models_deque.size(); ++k)
                            {
                                models_deque[k] += ModelModificationFromJson(Model::GetModel(model_name, custom), part["apply"]);
                                models_weights[k] *= model_weight;
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < models_weights.size(); ++i)
            {
                weights_sum += models_weights[i];
            }
        }
        models = std::vector<Model>(std::make_move_iterator(models_deque.begin()), std::make_move_iterator(models_deque.end()));
    }
    
#if PROTOCOL_VERSION < 347
    Blockstate::Blockstate(const int id_, const unsigned char metadata_,
                           const bool transparent_, const bool solid_, const bool fluid_,
                           const float hardness_, const TintType tint_type_, const std::string &name_,
                           const Model &model_) :
                           id(id_), metadata(metadata_), transparent(transparent_),solid(solid_),
                           hardness(hardness_), fluid(fluid_), tint_type(tint_type_), m_name(name_)
#else
    Blockstate::Blockstate(const int id_,
                           const bool transparent_, const bool solid_, const bool fluid_,
                           const float hardness_, const TintType tint_type_, const std::string &name_,
                           const Model &model_) :
                           id(id_), transparent(transparent_), solid(solid_), 
                           hardness(hardness_), fluid(fluid_), tint_type(tint_type_), m_name(name_)
#endif
    {
        weights_sum = 1;
        random_generator = std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count());

        models_weights = { 1 };
        models = { model_ };
    }

    const unsigned int Blockstate::GetId() const
    {
        return id;
    }

#if PROTOCOL_VERSION < 347
    const unsigned char Blockstate::GetMetadata() const
    {
        return metadata;
    }
#endif

    const Model &Blockstate::GetModel(const unsigned char index) const
    {
        return models[index];
    }

    const unsigned char Blockstate::GetRandomModelId(const Position* pos) const
    {
        int random_value = 0;
        if (!pos)
        {
            random_value = std::uniform_int_distribution<int>(0, weights_sum - 1)(random_generator);
        }
        else
        {
            // Faster pseudo random choice
            random_value = (pos->x + pos->y + pos->z) % weights_sum;
        }
        
        const size_t num_models = models_weights.size();
        for (int i = 0; i < num_models; ++i)
        {
            if (random_value < models_weights[i])
            {
                return static_cast<unsigned char>(i);
            }
            random_value -= models_weights[i];
        }
        //Should never be here
        return 0;
    }

    const std::string& Blockstate::GetName() const
    {
        return m_name;
    }

    const std::string& Blockstate::GetVariableValue(const std::string& variable) const
    {
        return variables.at(variable);
    }

    const bool Blockstate::IsAir() const
    {
        return id == 0;
    }

    const bool Blockstate::IsSolid() const
    {
        return solid;
    }

    const bool Blockstate::IsTransparent() const
    {
        return transparent;
    }

    const bool Blockstate::IsFluid() const
    {
        return fluid;
    }

    const float Blockstate::GetHardness() const
    {
        return hardness;
    }

    const TintType Blockstate::GetTintType() const
    {
        return tint_type;
    }

#if PROTOCOL_VERSION < 347
    const unsigned int Blockstate::IdMetadataToId(const unsigned int id_, const unsigned char metadata_)
    {
        return id_ << 4 | metadata_;
    }

    void Blockstate::IdToIdMetadata(const unsigned int input_id, unsigned int &output_id, unsigned char &output_metadata)
    {
        output_id = input_id >> 4;
        output_metadata = input_id & 0x0F;
    }
#endif

    void Blockstate::ClearCache()
    {
        cached_jsons.clear();
    }

#if USE_GUI
    void Blockstate::UpdateModelsWithAtlasData(const Renderer::Atlas* atlas)
    {
        for (auto& m : models)
        {
            for (auto& f : m.GetFaces())
            {
                // Extract texture info in the atlas
                std::array<unsigned short, 4> texture_pos = { 0, 0, 0, 0 };
                std::array<unsigned short, 4> texture_size = { 0, 0, 0, 0 };
                std::array<Renderer::Transparency, 2> transparencies = { Renderer::Transparency::Opaque, Renderer::Transparency::Opaque };
                std::array<Renderer::Animation, 2> animated = { Renderer::Animation::Static, Renderer::Animation::Static };

                for (int i = 0; i < std::min(2, static_cast<int>(f.texture_names.size())); ++i)
                {
                    const Renderer::TextureData& texture_data = atlas->GetData(f.texture_names[i]);
                    std::tie(texture_pos[2 * i + 0], texture_pos[2 * i + 1]) = texture_data.position;
                    std::tie(texture_size[2 * i + 0], texture_size[2 * i + 1]) = texture_data.size;
                    transparencies[i] = texture_data.transparency;
                    animated[i] = texture_data.animation;
                }

                // Main texture coords in the atlas
                std::array<float, 4> coords = f.face.GetTextureCoords(false);
                unsigned short height_normalizer = animated[0] == Renderer::Animation::Animated ? texture_size[0] : texture_size[1];
                coords[0] = (texture_pos[0] + coords[0] / 16.0f * texture_size[0]) / atlas->GetWidth();
                coords[1] = (texture_pos[1] + coords[1] / 16.0f * height_normalizer) / atlas->GetHeight();
                coords[2] = (texture_pos[0] + coords[2] / 16.0f * texture_size[0]) / atlas->GetWidth();
                coords[3] = (texture_pos[1] + coords[3] / 16.0f * height_normalizer) / atlas->GetHeight();
                f.face.SetTextureCoords(coords, false);

                // Overlay texture coords in the atlas if existing
                if (f.texture_names.size() > 1)
                {
                    coords = f.face.GetTextureCoords(true);
                    height_normalizer = animated[1] == Renderer::Animation::Animated ? texture_size[2] : texture_size[3];
                    coords[0] = (texture_pos[2] + coords[0] / 16.0f * texture_size[2]) / atlas->GetWidth();
                    coords[1] = (texture_pos[3] + coords[1] / 16.0f * height_normalizer) / atlas->GetHeight();
                    coords[2] = (texture_pos[2] + coords[2] / 16.0f * texture_size[2]) / atlas->GetWidth();
                    coords[3] = (texture_pos[3] + coords[3] / 16.0f * height_normalizer) / atlas->GetHeight();
                    f.face.SetTextureCoords(coords, true);
                }
            
                f.face.SetTransparencyData(transparencies[0]);
            }
        }
    }
#endif

    const int Blockstate::GetNumModels() const
    {
        return models.size();
    }
} //Botcraft