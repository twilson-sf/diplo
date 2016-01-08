#ifndef _MYSTRUCT_H_
#define _MYSTRUCT_H_

#include "vector"

class PSProvince;
class PSPower;
class PSUnit;

using namespace std;
typedef vector<PSProvince*> Provinces;

struct Retreat
{
	PSUnit* m_pUnit;
	PSProvince* m_pOrig;
	Provinces m_Possib;
};
	
struct Disband
{
	PSPower* m_pPower;
};

struct Build
{
public:
	PSPower* m_pPower;
private:
  Build * pBuild;
};

struct SC
{
	PSPower* m_pPower;
	PSProvince* m_pProvince;
};

typedef vector<Retreat*>		Retreats;
typedef vector<Disband*>		Disbands;
typedef vector<Build*>			Builds;
typedef vector<SC*>				SCs;

#endif // _MYSTRUCT_H_
