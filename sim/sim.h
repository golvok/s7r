#ifndef SIM_H
#define SIM_H

#include <stdlib.h>

class Sim {
public:
	Sim();
	bool update(size_t ticks);
private:
	Sim& operator=(const Sim&);
	Sim(const Sim&);
};

#endif /* SIM_H */
