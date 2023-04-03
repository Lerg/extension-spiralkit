#ifndef SK_HASHTABLE_H
#define SK_HASHTABLE_H

#include <dmsdk/dlib/hashtable.h>

namespace dmHashtableUtil {
	template <typename K, typename V>
	inline bool Exists(dmHashTable<K, V> *hashtable, K key) {
		return hashtable->Get(key) != nullptr;
	}

	template <typename K, typename V>
	struct FindKeyContext {
		const K *key;
		V value;
	};

	template <typename K, typename V>
	inline void OnFindKeyCallback(FindKeyContext<K, V> *context, const K *key, V *value) {
		if (*value == context->value) {
			context->key = key;
		}
	}

	template <typename K, typename V>
	inline const K *FindKey(dmHashTable<K, V> *hashtable, V value) {
		FindKeyContext<K, V> context;
		context.key = nullptr;
		context.value = value;
		hashtable->Iterate(OnFindKeyCallback, &context);
		return context.key;
	}
}

#endif
