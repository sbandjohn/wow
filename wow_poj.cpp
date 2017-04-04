#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>
#include <vector>
using namespace std;

const int mLen=200;

const int baseNumber=2;
const int baseNameLen=5;
enum BaseType{ red, blue };
char baseName[baseNumber][baseNameLen]={"red","blue"};

const int warriorNumber=5;
const int nameLen=10;
const int identityLen= baseNameLen+1+ nameLen + 10;
enum WarriorType{ dragon, ninja, iceman, lion, wolf};
char warriorName[warriorNumber][nameLen]={"dragon", "ninja","iceman","lion","wolf"};

class WarriorInfo{
public:
	int strength;
	int power;
}warriorInfo[warriorNumber];

WarriorType orderRed[warriorNumber]= {iceman, lion, wolf, ninja, dragon};
WarriorType orderBlue[warriorNumber]={lion, dragon, ninja, iceman, wolf};
	
class BattleField;
class Base;
class City;
class Warrior;

class MessageBox{
private:
	char msg[mLen];
public:
	int time;
public:
	MessageBox():time(0){}
	~MessageBox(){}
	void append(const char *s){
		int hour=time/60,minute=time%60;
		printf("%03d:%02d %s\n",hour,minute,s);
	}
};
MessageBox *msgBox;

class BattleField{
public:
	Base *baseRed,*baseBlue;
	int strength;
	int nCity;
	int timeLimit, time;
	
	vector<City*> city;
	
	friend class City;
	friend class Warrior;
	friend class Base;
	
public:
	BattleField(int nCity_,int strength_);
	~BattleField();
	
	void time00();
	void time10();
	void time20();
	void time30();
	void time40();
	void time50();
	
	void run(int limit);
	void end();
};

class Base{
friend class Warrior;
public:
	BattleField *battleField;
	BaseType type;
public:
	char name[baseNameLen];
	int location;
	int strength,add;
	int nEnemy;
	int cnt[warriorNumber];
	Warrior* warrior;
	int finishMaking;
	int taken;
	
	WarriorType order[warriorNumber];
	int total,cur;
	
public:
	Base(BaseType type_, int strength_, WarriorType* order_);
	~Base();
	int makeWarrior();
	void report();
	void award(Warrior* w);
	void attacked();
	void receive(int s);
	void accept();
	
};

class City{
public:
	enum FightRes{
		redWin,blueWin,draw,none
	};
	BattleField *battleField;
	int location;
	int strength;
	FightRes flag;
	FightRes preFight,curFight;
	Warrior *warriorRed,*warriorBlue;
	
public:
	City(int location_);
	~City();
	void fight(Warrior *a, Warrior *b);
	void tryFight();
	void tryTake();
	void tryRaise();
	void produce(int s);
};

class Warrior{
public:
	Base* belong;
	int location;
	int id;
	WarriorType type;
	
	int kill;
	char identity[identityLen];
	
public:
	int strength,power;
	
public:
	Warrior(){}
	virtual ~Warrior(){}
	
	virtual void born(Base* belong_, int location_, int id_, WarriorType type_);
	
	virtual void init(){}
	
	virtual void move(int newLocation){
		char msg[mLen];
		sprintf(msg,"%s marched to city %d with %d elements and force %d",
			identity, newLocation, strength, power);
		msgBox->append(msg);
		
		location=newLocation;
	}
	
	virtual void reach(Base* opposite){
		char msg[mLen];
		sprintf(msg,"%s reached %s headquarter with %d elements and force %d",identity, opposite->name, strength, power);
		msgBox->append(msg);
		
		opposite->attacked();
		
	}
	
	virtual void take(int s);
	
	virtual void attack(Warrior *enemy){
		char msg[mLen];
		sprintf(msg,"%s attacked %s in city %d with %d elements and force %d",
			identity, enemy->identity, location, strength, power);
		msgBox->append(msg);
		
		enemy->hurt(power);
	}
	
	virtual int alive(){
		return strength>0;
	}
	
	virtual void hurt(int power){
		strength-=power;
	}
	
	virtual void fightBack(Warrior *enemy){
		char msg[mLen];
		sprintf(msg, "%s fought back against %s in city %d",
			identity, enemy->identity, location);
		msgBox->append(msg);
		
		enemy->hurt(power/2);
	}
	
	virtual void active_win(Warrior *enemy){}
	virtual void active_draw(Warrior *enemy){}
	virtual void passive_win(Warrior *enemy){}
	virtual void passive_draw(Warrior *enemy){}
	virtual void lose(Warrior *enemy){
		kill=0;
		char msg[mLen];
		sprintf(msg, "%s was killed in city %d", identity, location);
		msgBox->append(msg);
	}
	virtual void active_finish(){}
	virtual void passive_finish(){}
	
};

void Warrior::born(Base* belong_, int location_, int id_, WarriorType type_){
	belong=belong_;
	location=location_;
	id=id_;
	type=type_;
	strength=warriorInfo[type].strength;
	power=warriorInfo[type].power;
	sprintf(identity,"%s %s %d",belong->name, warriorName[type], id);
	
	init();
	
	char msg[mLen];
	sprintf(msg, "%s born",identity);
	msgBox->append(msg);
}

class Dragon: public Warrior{
public:
	void yell(){
		char msg[mLen];
		sprintf(msg,"%s yelled in city %d",identity,location);
		msgBox->append(msg);
	}
	
	virtual void active_finish(){
		if (alive()) yell();
	}
};

class Ninja: public Warrior{
public:
	virtual void fightBack(Warrior *enemy){
		// this stupid guy doesn't fight back....
	}
};

class Iceman: public Warrior{
private:
	int moves;
public:
	virtual void init(){
		moves=0; // here's the adaptation
	}
	
	virtual void move(int newLocation){
		location=newLocation;
		++moves;
		if (moves%2 == 0){
			strength-=9;
			if (strength<=0) strength=1;
			power+=20;
		}
		
		char msg[mLen];
		sprintf(msg,"%s marched to city %d with %d elements and force %d",identity, location, strength, power);
		msgBox->append(msg);
	}
	
	virtual void reach(Base *opposite){
		++moves;
		if (moves%2==0){
			strength-=9;
			if (strength<=0) strength=1;
			power+=20;
		}
		
		char msg[mLen];
		sprintf(msg,"%s reached %s headquarter with %d elements and force %d",identity, opposite->name, strength, power);
		msgBox->append(msg);
		
		opposite->attacked();
	}
};

class Lion: public Warrior{
private:
	int pre_strength;
public:
	virtual void hurt(int power){
		pre_strength=strength;
		strength-=power;
	}
	virtual void lose(Warrior *enemy){
		char msg[mLen];
		sprintf(msg,"%s was killed in city %d",identity, location);
		msgBox->append(msg);
		
		enemy->strength += pre_strength;
	}
};

class Wolf: public Warrior{
private:
	int awc;
public:
	virtual void init(){
		awc=0;
	}
	virtual void active_win(Warrior *enemy){
		++awc;
		if (awc%2 == 0){
			strength*=2;
			power*=2;
		}
	}
};

int get(int T){
	int strength,n,t;
	if (scanf("%d%d%d",&strength,&n,&t)!=3) return 0;
	for (int i=0;i<warriorNumber;++i)
		scanf("%d",&warriorInfo[i].strength);
	for (int i=0;i<warriorNumber;++i)
		scanf("%d",&warriorInfo[i].power);
	printf("Case:%d\n",T);
	
	msgBox = new MessageBox;
	
	BattleField battleField(n,strength);
	
	battleField.run(t);

	return 1;
}

int main(){
	freopen("wow.in","r",stdin);
	freopen("wow.out","w",stdout);
	
	int T;
	scanf("%d",&T);
	
	for (int i=1;i<=T;++i) get(i);
	
	return 0;
}

BattleField::BattleField(int nCity_,int strength_){
	nCity=nCity_;
	city=vector<City*> (nCity+1);
	for (int i=1;i<=nCity;++i)
		city[i]= new City(i);
	strength=strength_;
	baseRed= new Base(red, strength, orderRed);
	baseBlue=new Base(blue,strength, orderBlue);
	time=0;	
}

BattleField::~BattleField(){
	delete baseRed;
	delete baseBlue;
	for (int i=1;i<=nCity;++i) delete city[i];
}

void BattleField::time00(){
	baseRed->makeWarrior();
	baseBlue->makeWarrior();
}

void BattleField::time10(){
	vector<Warrior*> tmpRed,tmpBlue;
	
	if (city[1]->warriorBlue){
		city[1]->warriorBlue->reach(baseRed);
		delete city[1]->warriorBlue;
	}
	
	if (baseRed->warrior){
		tmpRed.push_back(baseRed->warrior);
		baseRed->warrior->move(1);
	}
	
	for (int i=1;i<=nCity;++i){
		if (i>=2 && city[i-1]->warriorRed){
			tmpRed.push_back(city[i-1]->warriorRed);
			city[i-1]->warriorRed->move(i);
		}
		if (i<=nCity-1 && city[i+1]->warriorBlue){
			tmpBlue.push_back(city[i+1]->warriorBlue);
			city[i+1]->warriorBlue->move(i);
		}
	}
	
	if (baseBlue->warrior){
		tmpBlue.push_back(baseBlue->warrior);
		baseBlue->warrior->move(nCity);
	}
	
	if (city[nCity]->warriorRed){
		city[nCity]->warriorRed->reach(baseBlue);
		delete city[nCity]->warriorRed;
	}
	
	baseRed->warrior = baseBlue->warrior=0;
	
	for (int i=1;i<=nCity;i++)
		city[i]->warriorRed=city[i]->warriorBlue=0;
	
	for (vector<Warrior*>::iterator it=tmpRed.begin();it!=tmpRed.end();++it){
		city[(*it)->location]->warriorRed= *it;
	}

	for (vector<Warrior*>::iterator it=tmpBlue.begin();it!=tmpBlue.end();++it){
		city[(*it)->location]->warriorBlue= *it;
	}
}

void BattleField::time20(){
	for (int i=1;i<=nCity;++i) city[i]->produce(10);
}

void BattleField::time30(){
	for (int i=1;i<=nCity;++i) city[i]->tryTake();
	baseRed->accept();
	baseBlue->accept();
}

void BattleField::time40(){
	for (int i=1;i<=nCity;++i){
		city[i]->tryFight();
		city[i]->tryTake();
		city[i]->tryRaise();
	}
	for (int i=nCity;i>=1;--i)
		if (city[i]->warriorRed && city[i]->warriorRed->kill){
			baseRed->award(city[i]->warriorRed);
		}
	for (int i=1;i<=nCity;++i)
		if (city[i]->warriorBlue && city[i]->warriorBlue->kill){
			baseBlue->award(city[i]->warriorBlue);
		}
	baseRed->accept();
	baseBlue->accept();
}

void BattleField::time50(){
	baseRed->report();
	baseBlue->report();
}

void BattleField::run(int limit){
	for (int time=0;time<=limit;time+=10){
		msgBox->time = time;
		switch (time%60){
			case 0: time00(); break;
			case 10: time10(); break;
			case 20: time20(); break;
			case 30: time30(); break;
			case 40: time40(); break;
			case 50: time50(); break;
		}
		if (baseRed->taken || baseBlue->taken) break;
	}
}

Base::Base(BaseType type_, int strength_, WarriorType* order_){
	type=type_;
	strength=strength_;
	add=0;
	nEnemy=0;
	taken=0;
	for (int i=0;i<warriorNumber;++i){
		cnt[i]=0;
		order[i]=order_[i];
	}
	memcpy(name, baseName[type], sizeof(name));
	total=0;
	cur=0;
	finishMaking=0;
	warrior=0;
}

Base::~Base(){
	if (warrior) delete warrior;
}

int Base::makeWarrior(){
	WarriorType warriorType=order[cur];
	if (strength < warriorInfo[warriorType].strength) return 0;
	
	++total;
	Warrior* p = NULL;
	switch (warriorType) {
		case dragon: p=new Dragon; break;
		case ninja:  p=new Ninja; break;
		case iceman: p=new Iceman; break;
		case lion:   p=new Lion; break;
		case wolf:   p=new Wolf; break;
	}
	p->born(this, location, total, warriorType);
	++cnt[warriorType];
	strength-=warriorInfo[warriorType].strength;
	warrior=p;
	cur=(cur+1)%warriorNumber;
	return 1;
}

void Base::receive(int s){
	add+=s;
}

void Base::accept(){
	strength+=add;
	add=0;
}

void Base::report(){
	char msg[mLen];
	sprintf(msg,"%d elements in %s headquarter", strength, name);
	msgBox->append(msg);
}

void Base::award(Warrior* w){
	if (strength<8) return ;
	strength-=8;
	w->strength+=8;
}

void Base::attacked(){
	if (++nEnemy == 2){
		taken=1;
		char msg[mLen];
		sprintf(msg,"%s headquarter was taken",name);
		msgBox->append(msg);
	}
}

City::City(int location_){
	location=location_;
	strength=0;
	flag=none;
	preFight=curFight=none;
	warriorRed=warriorBlue=0;
}

City::~City(){
	if (warriorRed) delete warriorRed;
	if (warriorBlue) delete warriorBlue;
}

void City::tryRaise(){
	if (curFight==none) return ;
	if (curFight==preFight && curFight!=draw){
		if (flag!=curFight){
			char msg[mLen];
			sprintf(msg,"%s flag raised in city %d",baseName[curFight], location);
			msgBox->append(msg);
			flag=curFight;
		}
	}
}

void City::tryFight(){
	if (curFight!=none) preFight=curFight;
	curFight=none;
	if (warriorRed) warriorRed->kill=0;
	if (warriorBlue) warriorBlue->kill=0;
	if (warriorRed && warriorBlue){
		curFight=draw;
		if (flag==redWin || flag==none && (location%2))
			fight(warriorRed, warriorBlue);
		else
			fight(warriorBlue, warriorRed);
		
		if (warriorRed->kill) curFight=redWin;
		if (warriorBlue->kill)curFight=blueWin;
		
		if (!warriorRed->alive()){
			delete warriorRed;
			warriorRed=0;
		}
		if (!warriorBlue->alive()){
			delete warriorBlue;
			warriorBlue=0;
		}
	}
}

void City::tryTake(){
	if (strength==0) return ;
	if (warriorRed && !warriorBlue){
		warriorRed->take(strength);
		strength=0;
	}
	if (!warriorRed && warriorBlue){
		warriorBlue->take(strength);
		strength=0;
	}
}

void City::produce(int s){ strength+=s;}

void Warrior::take(int s){
	belong->receive(s);
	char msg[mLen];
	sprintf(msg,"%s earned %d elements for his headquarter",identity,s);
	msgBox->append(msg);
}

void City::fight(Warrior *a, Warrior *b){
	a->attack(b);
	if (b->alive()){
		b->fightBack(a);
		if (a->alive()){
			a->active_draw(b);
			b->passive_draw(a);
		}else{
			b->kill=1;
			b->passive_win(a);
			a->lose(b);
		}
	}else{
		a->kill=1;
		a->active_win(a);
		b->lose(a);
	}
	a->active_finish();
	b->passive_finish();
}