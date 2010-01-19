#pragma once

struct IDirect3DDevice9;
struct ID3DXEffect;
struct IDirect3DTexture9;
#include <d3dx9.h>

namespace rgde
{
	namespace render
	{
		namespace effects
		{
			typedef char* internal_effect_handle;

			typedef std::map<std::string, std::string> annotations_map;
			typedef annotations_map::iterator annot_iter;
			typedef annotations_map::const_iterator annot_const_iter;

			class BaseHandle
			{
				friend class Effect;			
			public:
				BaseHandle(){m_handle = 0;}
				virtual ~BaseHandle(){}
				const std::string GetName() const {return m_name;}

				virtual bool IsValid() const {return 0 != m_handle;}
				virtual void Refresh(Effect& e) = 0;

				bool IsAnnotationExist(const std::string& annot_name) const;
				std::string GetAnnotation(const std::string& annot_name) const;
				const annotations_map& GetAnnotations() const {return annotations;}

			protected:
				std::string m_name;			
				internal_effect_handle m_handle;
				annotations_map annotations;
			};

			typedef boost::shared_ptr<BaseHandle> BaseHandlePtr;
			typedef std::vector<BaseHandlePtr> BaseHandlesVertor;

			class ParamInfo : public BaseHandle
			{
				friend class Effect;
			public:
				typedef std::vector<char> data;
				enum Type
				{
					INT, STRING, FLOAT, FLOAT2, 
					FLOAT3, FLOAT4, TEXTURE, BOOL,
					INVALID
				};

				struct UIParams
				{
					UIParams();
					std::string Name;
					bool HasSlider;
					int SliderMax; //"UIMax"
					int SliderMin; //"UIMin"
					int SliderFactor; //"UIStep"
					bool isColorSwatch;
				};

				ParamInfo() {m_is_tweakable = false;}

				/// Parameter tweakble in engine, FX Composer, RenderMonkey, etc??
				bool IsTweakable() const {return m_is_tweakable;}
				const std::string& GetSemantic() const {return m_semantic;}

				virtual bool IsValid() const {return 0 != m_handle && m_is_used;}

				virtual void Refresh(Effect& e);	

				int GetColumns() const {return m_columns;}
				Type GetType() const {return m_type;}

				const data& GetData() const {return m_data;}

				const std::string& GetUIName() const {return m_ui_params.Name;}
				bool HasSlider() const {return m_ui_params.HasSlider;}
				const UIParams& GetUIParams() const {return m_ui_params;}
				const bool IsColorSwatch() const {return m_ui_params.isColorSwatch;}

				const D3DXPARAMETER_DESC& GetDxDesc() const {return m_dx_desc;}

			protected:
				D3DXPARAMETER_DESC m_dx_desc;
				data m_data;
				int m_columns;
				std::string m_semantic;
				bool m_is_used;
				bool m_is_tweakable;
				Type m_type;
				UIParams m_ui_params;
			};

			typedef boost::shared_ptr<ParamInfo> ParamPtr;
			typedef std::map<std::string, ParamPtr> ParamsMap;
			typedef ParamsMap::const_iterator ParamsIter;

			typedef std::vector<ParamPtr> ParamsVector;

			/// Type enumeration
			enum LightType{
				LIGHT_OMNI = (1<<0),
				LIGHT_SPOT = (1<<1),
				LIGHT_DIR = (1<<2),
				LIGHT_OMNI_PROJ = (1<<3),
				FORCE_DWORD    = 0x7fffffff,
			};

			struct TechInfo : public BaseHandle
			{
				TechInfo();
				friend class Effect;
				virtual void Refresh(Effect& e);			
				ParamsMap params;			
				float PixelShaderVersion;
				int LightTypes;
				int MaxLights;
				bool PerPixel;
				bool ShadowProjector;
				bool LightMapping;
				bool SupportsInstancing;
				bool UseVS;
				bool OverridesEngineMultipass;
				std::string Group;
				int tech_index; // должен оставаться неизменным иначе не будет работать перезагрузка!
			};

			typedef boost::shared_ptr<TechInfo> TechInfoPtr;

			typedef std::list<TechInfoPtr> TechList;
			typedef TechList::iterator TechListIter;
			typedef TechList::const_iterator TechListConstIter;

			typedef std::map<std::string, TechInfoPtr> TechMap;
			typedef TechMap::iterator TechMapIter;
			typedef TechMap::const_iterator TechMapConstIter;

			class ParamBlock
			{
				friend class Effect;
				ParamBlock(Effect& parent_effect);
			public:			
				~ParamBlock();

				void Begin();
				void End();

				void Apply();

				/// returns NULL if Effect was killed.
				const Effect* GetParentEffect() const {return m_parent_effect;}

			private:
				void DeleteHandle();

			private:
				Effect* m_parent_effect;
				internal_effect_handle m_handle;
			};

			typedef boost::shared_ptr<ParamBlock> ParamBlockPtr;

			class Effect
			{
				friend ParamInfo;
				friend TechInfo;
				friend ParamBlock;
				typedef boost::weak_ptr<ParamBlock> ParamBlockWeakPtr;
				typedef std::list<ParamBlockWeakPtr> ParamBlocks;
				typedef ParamBlocks::iterator ParamBlockIter;

			public:
				Effect(ID3DXEffect* effect, float shader_max_version = 2.2f);
				~Effect();

				TechInfoPtr GetTechnique(const std::string& tech_name);
				const TechList& GetTechniques() const {return m_techniques_list;}

				void SetTechnique(TechInfoPtr tech);

				void Reload(ID3DXEffect *new_effect = NULL);

				ParamPtr GetParameterByName(const std::string& param_name) const;
				ParamPtr GetParameterBySemantic(const std::string& param_name) const;

				ParamBlockPtr CreateParamBlock();

				bool SetTechnique(const std::string& tech_name);
				unsigned int Begin(unsigned int flags);
				bool BeginPass(unsigned int pass);
				bool EndPass();
				bool End();

				bool SetInt(const ParamPtr& param, int value);
				bool SetBool(const ParamPtr& param, bool value);
				bool SetFloat(const ParamPtr& param, float value);
				bool SetTexture(const ParamPtr& param, IDirect3DBaseTexture9* value);
				bool SetMatrix(const ParamPtr& param, const D3DXMATRIX& value);
				bool SetVector(const ParamPtr& param, const D3DXVECTOR4& value);
				bool SetFloatArray(const ParamPtr& param, const float* pf, unsigned int Count);
				bool SetVectorArray(const ParamPtr& param, const D3DXVECTOR4* pVector, unsigned int Count);
				bool set_matrix_array(const ParamPtr& param, const D3DXMATRIX* pMatrix, unsigned int Count);
				bool SetValue(const ParamPtr& param, void* data, unsigned int bytes);

				bool SetInt(const std::string& param_name, int value);			
				bool SetBool(const std::string& param_name, bool value);
				bool SetFloat(const std::string& param_name, float value);
				bool SetTexture(const std::string& param_name, IDirect3DBaseTexture9* value);
				bool SetMatrix(const std::string& param_name, const D3DXMATRIX& value);
				bool SetVector(const std::string& param_name, const D3DXVECTOR4& value);
				bool SetFloatArray(const std::string& param_name, const float* pf, unsigned int Count);
				bool SetVectorArray(const std::string& param_name, const D3DXVECTOR4* pVector, unsigned int Count);
				bool set_matrix_array(const std::string& param_name, const D3DXMATRIX* pMatrix, unsigned int count);
				bool SetValue(const std::string& param_name, void* data, unsigned int bytes);

				void commit_changes();

				void on_device_reset();
				void on_device_lost();

				const ParamsVector& GetParams() const {return m_params;}

				bool IsReadsColorBuffer() const {return m_ReadsColorBuffer;}
				bool IsReadsGlobalColorBuffer() const {return m_ReadsGlobalColorBuffer;}
				bool IsForceLDR() const {return m_ForceLDR;}
				bool IsUsesLightArray() const {return m_UsesLightArray;}

				static void SetSkipUnusedShaderParams(bool val) {skip_unused_params = val;}

			private:
				void clean_param_blocks();
				void init(float shader_max_version);
				void parse_techniques(std::list<TechInfoPtr>& tech_list, float shader_max_version);
				void parse_params(std::list<ParamPtr>& params_list);			
				void erase_param_block(ParamBlockIter iter);
				void FillUIParams(std::list<ParamPtr>& params_list);

			private:
				bool m_ReadsColorBuffer;
				bool m_ReadsGlobalColorBuffer;
				bool m_ForceLDR;
				bool m_UsesLightArray;
				static bool skip_unused_params;

				ParamsVector m_params;
				ParamsMap m_params_by_name;
				ParamsMap m_params_by_semantic;
				ParamBlocks	m_param_blocks;
				ID3DXEffect* m_effect;
				TechList m_techniques_list;
				TechMap m_techiques;
				BaseHandlesVertor m_handlers; // need to call refresh after reload
			};
		}
	}
}