#ifndef SK_ARRAY_H
#define SK_ARRAY_H

#include <dmsdk/dlib/array.h>
#include <dmsdk/dlib/log.h>

namespace dmArrayUtil {
	template <typename T>
	static void Insert(dmArray<T> *array, uint32_t index, T element) {
		assert(array->Capacity() > array->Size());
		array->SetSize(array->Size() + 1);
		for (uint32_t i = index; i < array->Size() - 1; ++i) {
			(*array)[i + 1] = (*array)[i];
		}
		(*array)[index] = element;
	}

	template <typename T>
	static void Remove(dmArray<T> *array, uint32_t index) {
		assert(index < array->Size());
		for (uint32_t i = index; i < array->Size() - 1; ++i) {
			(*array)[i] = (*array)[i + 1];
		}
		array->SetSize(array->Size() - 1);
	}

	template <typename T>
	static uint32_t GetIndexOf(dmArray<T> *array, T element) {
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
	static bool Exists(dmArray<T> *array, T element) {
		for (uint32_t i = 0; i < array->Size(); ++i) {
			if ((*array)[i] == element) {
				return true;
			}
		}
		return false;
	}

	/*Crashes
	template <typename T>
	static void Sort(dmArray<T> *array, int (*sort_function)(const void *, const void *)) {
		qsort(array->Begin(), array->Size(), sizeof(T), sort_function);
	}*/
}

#endif
