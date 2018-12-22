#ifndef PORTION_H
#define PORTION_H

#include <QString>

class Portion {
	QString m_foodName;
	uint m_grams;
	float m_kj;

  public:
	Portion();
	Portion( QString foodName, uint grams );
	QString getFoodName();
	float getKj();
};

#endif // PORTION_H
