#ifndef SK_ARRAY_H
#define SK_ARRAY_H

#include <dmsdk/dlib/array.h>
#include <dmsdk/dlib/log.h>

namespace dmArrayUtil {
	template <typename T>
	inline void Insert(dmArray<T> *array, uint32_t index, T element) {
		assert(array->Capacity() > array->Size());
		array->SetSize(array->Size() + 1);
		for (uint32_t i = index; i < array->Size() - 1; ++i) {
			(*array)[i + 1] = (*array)[i];
		}
		(*array)[index] = element;
	}

	template <typename T>
	inline void Remove(dmArray<T> *array, uint32_t index) {
		assert(index < array->Size());
		for (uint32_t i = index; i < array->Size() - 1; ++i) {
			(*array)[i] = (*array)[i + 1];
		}
		array->SetSize(array->Size() - 1);
	}

	template <typename T>
	inline void Move(dmArray<T> *array, uint32_t from_index, uint32_t to_index) {
		assert(from_index < array->Size());
		assert(to_index < array->Size());
		dmArray<T> &array_ref = *array;
		const T element = array_ref[from_index];
		if (from_index < to_index) {
			// Shift elements right
			for (uint32_t i = from_index; i < to_index; ++i) {
				array_ref[i] = array_ref[i + 1];
			}
		} else {
			// Shift elements left
			for (uint32_t i = from_index; i > to_index; --i) {
				array_ref[i] = array_ref[i - 1];
			}
		}
		array_ref[to_index] = element;
	}

	template <typename T>
	inline uint32_t GetIndexOf(dmArray<T> *array, T element) {
		for (uint32_t i = 0; i < array->Size(); ++i) {
			if ((*array)[i] == element) {
				return i;
			}
		}
		dmLogFatal("GetIndexOf: no such element.");
		assert(false);
		return 0;
	}

	template <typename T>
	inline bool Exists(dmArray<T> *array, T element) {
		for (uint32_t i = 0; i < array->Size(); ++i) {
			if ((*array)[i] == element) {
				return true;
			}
		}
		return false;
	}

	/*Crashes
	template <typename T>
	inline void Sort(dmArray<T> *array, int (*sort_function)(const void *, const void *)) {
		qsort(array->Begin(), array->Size(), sizeof(T), sort_function);
	}*/
}

#endif
