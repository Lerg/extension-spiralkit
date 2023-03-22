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

	static dmGameObject::CreateResult CompSpiralkitAddToUpdate(const dmGameObject::ComponentAddToUpdateParams& params) {
		return dmGameObject::CREATE_RESULT_OK;
	}

	static dmGameObject::UpdateResult CompSpiralkitOnUpdate(const dmGameObject::ComponentsUpdateParams &params, dmGameObject::ComponentsUpdateResult &result) {
		spiralkit::Spiralkit::OnUpdate(params.m_UpdateContext->m_DT);
		return dmGameObject::UPDATE_RESULT_OK;
	}

	static dmGameObject::InputResult CompSpiralkitOnInput(const dmGameObject::ComponentOnInputParams &params) {
		dmGameObject::InputResult result = dmGameObject::INPUT_RESULT_IGNORED;
		spiralkit::Spiralkit::OnInput(params);
		return result;
	}

	dmGameObject::CreateResult CompSpiralkitCreate(const dmGameObject::ComponentCreateParams &params) {
		spiralkit::Spiralkit::SetInstance(params.m_Instance);
		return dmGameObject::CREATE_RESULT_OK;
	}

	dmGameObject::CreateResult CompSpiralkitDestroy(const dmGameObject::ComponentDestroyParams &params) {
		return dmGameObject::CREATE_RESULT_OK;
	}

	static dmGameObject::Result CompTypeSpiralkitCreate(const dmGameObject::ComponentTypeCreateCtx *ctx, dmGameObject::ComponentType *type) {
		SpiralkitContext *spiralkitctx = new SpiralkitContext;
		spiralkitctx->m_Factory = ctx->m_Factory;

		ComponentTypeSetPrio(type, 1500);

		ComponentTypeSetContext(type, spiralkitctx);
		//ComponentTypeSetHasUserData(type, true);
		ComponentTypeSetReadsTransforms(type, false);

		ComponentTypeSetCreateFn(type, CompSpiralkitCreate);
		ComponentTypeSetDestroyFn(type, CompSpiralkitDestroy);
		ComponentTypeSetAddToUpdateFn(type, CompSpiralkitAddToUpdate);
		ComponentTypeSetUpdateFn(type, CompSpiralkitOnUpdate);
		ComponentTypeSetOnInputFn(type, CompSpiralkitOnInput);

		//ComponentTypeSetNewWorldFn(type, CompSpiralkitNewWorld);
		//ComponentTypeSetDeleteWorldFn(type, CompSpiralkitDeleteWorld);
		//ComponentTypeSetInitFn(type, CompSpiralkitInit);
		//ComponentTypeSetFinalFn(type, CompSpiralkitFinal);
		//ComponentTypeSetRenderFn(type, CompSpiralkitRender);
		//ComponentTypeSetOnMessageFn(type, CompSpiralkitOnMessage);
		//ComponentTypeSetOnReloadFn(type, CompSpiralkitOnReload);
		//ComponentTypeSetSetPropertiesFn(type, CompSpiralkitSetProperties);
		//ComponentTypeSetGetPropertyFn(type, CompSpiralkitGetProperty);
		//ComponentTypeSetSetPropertyFn(type, CompSpiralkitSetProperty);
		//ComponentTypeSetPropertyIteratorFn(type, CompSpiralkitIterProperties);
		//ComponentTypeSetGetFn(type, CompSpiralkitGetComponent);

		return dmGameObject::RESULT_OK;
	}

	static dmGameObject::Result CompTypeSpiralkitDestroy(const dmGameObject::ComponentTypeCreateCtx *ctx, dmGameObject::ComponentType *type) {
		SpiralkitContext *spiralkitctx = (SpiralkitContext*)ComponentTypeGetContext(type);
		delete spiralkitctx;
		return dmGameObject::RESULT_OK;
	}

	static dmGameObject::Result ComponentType_Destroy(const dmGameObject::ComponentTypeCreateCtx *ctx, dmGameObject::ComponentType *type) {
		SpiralkitContext *spiralkitctx = (SpiralkitContext*)ComponentTypeGetContext(type);
		delete spiralkitctx;
		return dmGameObject::RESULT_OK;
	}
}

#endif
