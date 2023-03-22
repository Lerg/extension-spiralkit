#ifndef SK_TIMEOUT_H
#define SK_TIMEOUT_H

#include <dmsdk/dlib/array.h>

namespace spiralkit {
	class Timeout {
		private:
			dmArray<float> timeoutDurations, timeoutRemainings;
			dmArray<bool> timeoutAutoremoves;
			uint32_t nextId = 0;

		public:
			Timeout(uint32_t capacity) {
				timeoutDurations.SetCapacity(capacity);
				timeoutRemainings.SetCapacity(capacity);
				timeoutAutoremoves.SetCapacity(capacity);
				timeoutDurations.SetSize(capacity);
				timeoutRemainings.SetSize(capacity);
				timeoutAutoremoves.SetSize(capacity);
				for (uint32_t i = 0; i < capacity; ++i) {
					timeoutDurations[i] = 0;
					timeoutRemainings[i] = 0;
					timeoutAutoremoves[i] = true;
				}
				nextId = capacity;
			}

			inline uint32_t Add() {
				const uint32_t id = nextId++;
				if (id == timeoutDurations.Size()) {
					if (timeoutDurations.Full()) {
						timeoutDurations.OffsetCapacity(16);
						timeoutRemainings.OffsetCapacity(16);
						timeoutAutoremoves.OffsetCapacity(16);
					}
					timeoutDurations.Push(0);
					timeoutRemainings.Push(0);
					timeoutAutoremoves.Push(true);
				}
				return id;
			}

			inline void Set(uint32_t id, float duration, bool autoremove = true) {
				if (timeoutDurations.Size() > id) {
					timeoutDurations[id] = duration;
					timeoutRemainings[id] = duration;
					timeoutAutoremoves[id] = autoremove;
				}
			}

			inline void Remove(uint32_t id) {
				if (timeoutDurations.Size() > id && timeoutDurations[id] != 0) {
					timeoutDurations[id] = 0;
					timeoutRemainings[id] = 0;
					timeoutAutoremoves[id] = true;
				}
			}

			inline bool Has(uint32_t id) {
				return timeoutDurations.Size() > id && timeoutDurations[id] != 0;
			}

			inline bool IsActive(uint32_t id) {
				return timeoutDurations.Size() > id && timeoutDurations[id] != 0 && timeoutRemainings[id] > 0;
			}

			inline bool IsOver(uint32_t id) {
				return timeoutDurations.Size() > id && timeoutDurations[id] != 0 && timeoutRemainings[id] <= 0;
			}

			inline float GetProgress(uint32_t id) {
				if (timeoutDurations.Size() > id && timeoutDurations[id] != 0) {
					const float duration = timeoutDurations[id];
					return (duration - timeoutRemainings[id]) / duration;
				}
				return 0;
			}

			void Update(float dt) {
				for (uint32_t i = 0; i < timeoutRemainings.Size(); ++i) {
					float remaining = timeoutRemainings[i];
					if (remaining > 0) {
						remaining -= dt;
						if (remaining < 0 && timeoutAutoremoves[i]) {
							Remove(i);
						} else {
							timeoutRemainings[i] = remaining;
						}
					}
				}
			}
	};
}

#endif
