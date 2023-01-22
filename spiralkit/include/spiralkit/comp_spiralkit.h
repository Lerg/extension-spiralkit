#ifndef SK_COMP_SPIRALKIT_H
#define SK_COMP_SPIRALKIT_H

#include <dmsdk/gameobject/gameobject.h>
#include <dmsdk/gameobject/component.h>
#include <dmsdk/dlib/log.h>
#include <spiralkit/spiralkit.h>

namespace spiralkit {
	struct SpiralkitContext {
		SpiralkitContext() {
			memset(this, 0, sizeof(*this));
		}
		dmResource::HFactory m_Factory;
	};

	struct SpiralkitComponent {
		dmGameObject::HInstance m_Instance;
		uint32_t m_MixedHash;
		uint16_t m_ComponentIndex;
		uint8_t m_Enabled : 1;
		uint8_t m_DoRender : 1;
		uint8_t m_AddedToUpdate : 1;
		uint8_t m_ReHash : 1;
	};

	static dmGameObject::CreateResult CompSpiralkitAddToUpdate(const dmGameObject::ComponentAddToUpdateParams& params) {
		return dmGameObject::CREATE_RESULT_OK;
	}

	static dmGameObject::UpdateResult CompSpiralkitOnUpdate(const dmGameObject::ComponentsUpdateParams& params, dmGameObject::ComponentsUpdateResult& result) {
		spiralkit::Spiralkit::OnUpdate(params.m_UpdateContext->m_DT);
		return dmGameObject::UPDATE_RESULT_OK;
	}

	static dmGameObject::InputResult CompSpiralkitOnInput(const dmGameObject::ComponentOnInputParams &params) {
		dmGameObject::InputResult result = dmGameObject::INPUT_RESULT_IGNORED;
		spiralkit::Spiralkit::OnInput(params);
		return result;
	}

	dmGameObject::CreateResult CompSpiralkitCreate(const dmGameObject::ComponentCreateParams& params) {
		return dmGameObject::CREATE_RESULT_OK;
	}

	dmGameObject::CreateResult CompSpiralkitDestroy(const dmGameObject::ComponentDestroyParams& params) {
		return dmGameObject::CREATE_RESULT_OK;
	}

	static dmGameObject::Result CompTypeSpiralkitCreate(const dmGameObject::ComponentTypeCreateCtx* ctx, dmGameObject::ComponentType* type) {
		SpiralkitContext* spiralkitctx = new SpiralkitContext;
		spiralkitctx->m_Factory = ctx->m_Factory;

		// Component type setup

		// Ideally, we'd like to move this priority a lot earlier
		// We sould be able to avoid doing UpdateTransforms again in the Render() function
		ComponentTypeSetPrio(type, 1500);

		ComponentTypeSetContext(type, spiralkitctx);
		//ComponentTypeSetHasUserData(type, true);
		ComponentTypeSetReadsTransforms(type, false);

		//ComponentTypeSetNewWorldFn(type, CompSpiralkitNewWorld);
		//ComponentTypeSetDeleteWorldFn(type, CompSpiralkitDeleteWorld);
		ComponentTypeSetCreateFn(type, CompSpiralkitCreate);
		ComponentTypeSetDestroyFn(type, CompSpiralkitDestroy);
		//ComponentTypeSetInitFn(type, CompSpineModelInit);
		//ComponentTypeSetFinalFn(type, CompSpineModelFinal);
		ComponentTypeSetAddToUpdateFn(type, CompSpiralkitAddToUpdate);
		ComponentTypeSetUpdateFn(type, CompSpiralkitOnUpdate);
		//ComponentTypeSetRenderFn(type, CompSpineModelRender);
		//ComponentTypeSetOnMessageFn(type, CompSpineModelOnMessage);
		ComponentTypeSetOnInputFn(type, CompSpiralkitOnInput);
		//ComponentTypeSetOnReloadFn(type, CompSpineModelOnReload);
		//ComponentTypeSetSetPropertiesFn(type, CompSpineModelSetProperties);
		//ComponentTypeSetGetPropertyFn(type, CompSpineModelGetProperty);
		//ComponentTypeSetSetPropertyFn(type, CompSpineModelSetProperty);
		//ComponentTypeSetPropertyIteratorFn(type, CompSpineModelIterProperties);
		//ComponentTypeSetGetFn(type, CompSpineModelGetComponent);

		return dmGameObject::RESULT_OK;
	}

	static dmGameObject::Result CompTypeSpiralkitDestroy(const dmGameObject::ComponentTypeCreateCtx* ctx, dmGameObject::ComponentType* type) {
		SpiralkitContext *spiralkitctx = (SpiralkitContext*)ComponentTypeGetContext(type);
		delete spiralkitctx;
		return dmGameObject::RESULT_OK;
	}

	static dmGameObject::Result ComponentType_Destroy(const dmGameObject::ComponentTypeCreateCtx* ctx, dmGameObject::ComponentType* type) {
		SpiralkitContext *spiralkitctx = (SpiralkitContext*)ComponentTypeGetContext(type);
		delete spiralkitctx;
		return dmGameObject::RESULT_OK;
	}
}

#endif
