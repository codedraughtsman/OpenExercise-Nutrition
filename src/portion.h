#ifndef PORTION_H
#define PORTION_H

#include <QString>

class Portion {

  public:
	QString m_foodName;
	uint m_xValue, m_yValue;

	Portion();
	Portion( QString foodName, uint xValue, uint yValue );
};

#endif // PORTION_H
