#ifndef PORTION_H
#define PORTION_H

#include <QString>

class Portion {

  public:
	QString m_foodName;
	double m_xValue, m_yValue;

	Portion();
	Portion( QString foodName, double xValue, double yValue );
};

#endif // PORTION_H
