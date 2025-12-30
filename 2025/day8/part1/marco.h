
// Define MERGESORT macro for sorting lists
// Usage: MERGESORT(type, cmp(type*, type *))
// Type must be a typedef type
// The type must be structured as below:
// typedef struct mystruct {
//	struct mystruct* next; 
//	int other memberfields
// } mystruct_t
// The comparator cmp(type* left, type* right) must return 0 if right is sorted before left,
// 1 otherwise

// For an annotated version, please checkout day 5,
// which contains a non-macro version


// Merge sort
#define MERGESORT(T, CMP) \
T* T##_sortlist(T* r) \
{ \
	if (r == NULL) return r; \
	if (r->next == NULL) return r; \
	int i = 0; \
	T *right = NULL; \
	T *left = NULL; \
	while (r) { \
		T *n = r->next; \
		if ((i++) % 2) { \
			r->next = left; \
			left = r; \
		} \
		else { \
			r->next = right; \
			right = r; \
		} \
		r = n; \
	} \
	left = T##_sortlist(left); \
	right = T##_sortlist(right); \
	T **rtarget = &r; \
	while (left || right) { \
		if (left && right) { \
			if (CMP(left, right)) { \
				*rtarget = left; \
				rtarget = &(left->next); \
				left = left->next; \
			} \
			else { \
				*rtarget = right; \
				rtarget = &(right->next); \
				right = right->next; \
			} \
		} \
		else if (left) { \
			*rtarget = left; \
			rtarget = &(left->next); \
			left = left->next; \
		} \
		else { \
			*rtarget = right; \
			rtarget = &(right->next); \
			right = right->next; \
		} \
	} \
	*rtarget = NULL; \
	return r; \
}
