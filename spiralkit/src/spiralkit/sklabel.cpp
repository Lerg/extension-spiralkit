#include <gamesys/label_ddf.h>
#include <dmsdk/dlib/log.h>
#include <spiralkit/hashes.h>
#include <spiralkit/defold.h>

#include <spiralkit/sklabel.h>

namespace spiralkit {
	SkLabel::SkLabel(asset::Label asset, SkObject *parent) {
		identifier = Defold::FactoryCreate(asset.path);
		this->parent = parent;
		instance = dmGameObject::GetInstanceFromIdentifier(Spiralkit::collection, identifier);

		Init();
		dmMessage::ResetURL(&componentUrl);
		componentUrl.m_Socket = Spiralkit::urlSocket;
		componentUrl.m_Path = identifier;
		componentUrl.m_Fragment = hashes::component;
	}

	void SkLabel::SetText(const char *text) {
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

	void SkLabel::SetTextF(const char *format, ...) {
		char text[512];
		va_list args;
		va_start(args, format);
		vsnprintf(text, sizeof(text), format, args);
		SetText(text);
		va_end (args);
	}
}
