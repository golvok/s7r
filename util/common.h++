#define DECL_COPYCON_AND_ASSIGNOP(clazz) private: clazz& operator=(const clazz&); clazz(const clazz&);
