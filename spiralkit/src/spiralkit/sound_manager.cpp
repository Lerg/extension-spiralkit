#include <gamesys/gamesys_ddf.h>
#include <spiralkit/dmsdk.h>
#include <spiralkit/spiralkit.h>

#include <spiralkit/sound_manager.h>

namespace spiralkit {
	dmhash_t SoundManager::_assetPath;
	bool SoundManager::isMusicOn = true;
	bool SoundManager::isSoundOn = true;

	void SoundManager::Play(asset::Sound sound) {
		if (!isSoundOn) {
			return;
		}
		dmMessage::URL sender;
		dmMessage::ResetURL(&sender);
		sender.m_Socket = Spiralkit::urlSocket;
		sender.m_Path = _assetPath;

		dmMessage::URL receiver;
		dmMessage::ResetURL(&receiver);
		receiver.m_Socket = Spiralkit::urlSocket;
		receiver.m_Path = _assetPath;
		receiver.m_Fragment = sound.name_hash;

		uint32_t play_id = dmSound::INVALID_PLAY_ID;
		dmGameSystemDDF::PlaySound message;
		message.m_Delay = 0;
		message.m_Gain = 1;
		message.m_Pan = 0;
		message.m_Speed = 1;
		message.m_PlayId = play_id;

		int32_t function_ref = 0;

		dmMessage::Post(
			&sender, &receiver, dmGameSystemDDF::PlaySound::m_DDFDescriptor->m_NameHash,
			(uintptr_t)Spiralkit::instance, (uintptr_t)function_ref,
			(uintptr_t)dmGameSystemDDF::PlaySound::m_DDFDescriptor,
			&message, sizeof(message), 0
		);
	}

	void SoundManager::Stop(asset::Sound sound) {
		dmMessage::URL sender;
		dmMessage::ResetURL(&sender);
		sender.m_Socket = Spiralkit::urlSocket;
		sender.m_Path = _assetPath;

		dmMessage::URL receiver;
		dmMessage::ResetURL(&receiver);
		receiver.m_Socket = Spiralkit::urlSocket;
		receiver.m_Path = _assetPath;
		receiver.m_Fragment = sound.name_hash;

		uint32_t play_id = dmSound::INVALID_PLAY_ID;
		dmGameSystemDDF::StopSound message;

		dmMessage::Post(
			&sender, &receiver, dmGameSystemDDF::StopSound::m_DDFDescriptor->m_NameHash,
			(uintptr_t)Spiralkit::instance, 0,
			(uintptr_t)dmGameSystemDDF::StopSound::m_DDFDescriptor,
			&message, sizeof(message), 0
		);
	}
}
