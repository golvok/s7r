#include <stdlib.h>

class Sim {
public:
	Sim();
	bool update(size_t ticks);
private:
	Sim& operator=(const Sim&);
	Sim(const Sim&);
};
