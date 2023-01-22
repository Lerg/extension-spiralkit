#ifndef DM_GAMESYS_RES_SIMPLE_DATA_H
#define DM_GAMESYS_RES_SIMPLE_DATA_H

#include <dmsdk/resource/resource.h>
#include <dmsdk/ddf/ddf.h>

#include "spiralkit_ddf.h" // generated from the spiralkit_ddf.proto

namespace spiralkit {
	struct SpiralkitResource {
		dmGameSystemDDF::SpiralkitDesc *m_DDF;
	};

	static dmResource::Result ResourceCreate(const dmResource::ResourceCreateParams &params) {
		dmGameSystemDDF::SpiralkitDesc *desc;
		dmDDF::Result e = dmDDF::LoadMessage(params.m_Buffer, params.m_BufferSize, &desc);
		if (e != dmDDF::RESULT_OK) {
			return dmResource::RESULT_FORMAT_ERROR;
		}
		// We hand out a pointer to a pointer, in order to be able to recreate the assets
		// when hot reloading.
		SpiralkitResource *resource = new SpiralkitResource;
		resource->m_DDF = desc;
		params.m_Resource->m_Resource = (void*)resource;
		return dmResource::RESULT_OK;
	}

	static dmResource::Result ResourceDestroy(const dmResource::ResourceDestroyParams &params) {
		SpiralkitResource *resource = (SpiralkitResource*)params.m_Resource->m_Resource;
		dmDDF::FreeMessage(resource->m_DDF);
		delete resource;
		return dmResource::RESULT_OK;
	}

	static dmResource::Result ResourceRecreate(const dmResource::ResourceRecreateParams &params) {
		dmGameSystemDDF::SpiralkitDesc *desc;
		dmDDF::Result e = dmDDF::LoadMessage(params.m_Buffer, params.m_BufferSize, &desc);
		if (e != dmDDF::RESULT_OK) {
			return dmResource::RESULT_FORMAT_ERROR;
		}
		SpiralkitResource *resource = (SpiralkitResource*)params.m_Resource->m_Resource;
		dmDDF::FreeMessage(resource->m_DDF);
		resource->m_DDF = desc;
		return dmResource::RESULT_OK;
	}

	static dmResource::Result ResourceType_Register(dmResource::ResourceTypeRegisterContext &ctx) {
		return dmResource::RegisterType(
			ctx.m_Factory,
			ctx.m_Name,
			0, // context
			0, // preload
			ResourceCreate,
			0, // post create
			ResourceDestroy,
			ResourceRecreate
		);
	}
}

#endif
