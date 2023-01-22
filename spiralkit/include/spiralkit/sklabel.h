#ifndef SK_SKLABEL_H
#define SK_SKLABEL_H

#include <gamesys/label_ddf.h>
#include "asset.h"
#include "skobject.h"

namespace spiralkit {
	class SkLabel : public SkObject {
		public:
			dmMessage::URL componentUrl;
			SkLabel(asset::Label asset, SkObject *parent = nullptr) {
				identifier = Defold::FactoryCreate(asset.path);
				this->parent = parent;
				const dmGameObject::HInstance go_instance = dmScript::CheckGOInstance(spiralkit::Defold::L);
				const dmGameObject::HCollection collection = dmGameObject::GetCollection(go_instance);
				instance = dmGameObject::GetInstanceFromIdentifier(collection, identifier);

				Init();
				dmMessage::ResetURL(&componentUrl);
				componentUrl.m_Socket = dmGameObject::GetMessageSocket(collection);
				componentUrl.m_Path = identifier;
				componentUrl.m_Fragment = hashes::component;
			}

			void SetText(const char *text) {
				const uint32_t text_length = strnlen(text, Defold::DM_MESSAGE_MAX_DATA_SIZE);
				const uint32_t data_size = sizeof(dmGameSystemDDF::SetText) + text_length + 1;
				if (data_size > Defold::DM_MESSAGE_MAX_DATA_SIZE) {
					dmLogError("The label string is too long!");
					return;
				}
				uint8_t data[data_size];

				dmGameSystemDDF::SetText *message = (dmGameSystemDDF::SetText*)data;
				message->m_Text = (const char*)sizeof(dmGameSystemDDF::SetText);
				memcpy((void*)(data + sizeof(dmGameSystemDDF::SetText)), text, text_length + 1);

				const dmMessage::Result r = dmMessage::Post(
					&url, &componentUrl, dmGameSystemDDF::SetText::m_DDFDescriptor->m_NameHash,
					(uintptr_t)instance, 0, (uintptr_t)dmGameSystemDDF::SetText::m_DDFDescriptor,
					data, data_size, 0
				);
				if (r != dmMessage::RESULT_OK) {
					dmLogError("Failed to send SetText message");
				}
			}

			void SetTextF(const char *format, ...) {
				char text[512];
  				va_list args;
				va_start(args, format);
				vsprintf(text, format, args);
				SetText(text);
				va_end (args);
			}
	};
}

#endif
