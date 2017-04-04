//#define debug
//#define local
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
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

const int weaponNumber=3;
const int weaponNameLen=6;
enum WeaponType{ sword, bomb, arrow};
WeaponType weaponMap[weaponNumber]={sword, bomb, arrow};
char weaponName[weaponNumber][weaponNameLen]={"sword","bomb","arrow"};

int arrowPower;

int loyaltyDecrease;

class BattleField;
class Base;
class City;
class Weapon;
class Warrior;

class MessageBox{
private:
	char msg[mLen];
public:
	int time;
	int needTime;
public:
	MessageBox():time(0),needTime(1){}
	~MessageBox(){}
	void noTime(){ needTime=0;}
	void showTime(){ needTime=1;}
	void append(const char *s){
		if (needTime){
			int hour=time/60,minute=time%60;
			printf("%03d:%02d %s\n",hour,minute,s);
		}else
			printf("%s\n",s);
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
	void time05();
	void time10();
	void time20();
	void time30();
	void time35();
	void time38();
	void time40();
	void time50();
	void time55();
	
	void run(int limit);
	void end();
	
	void print();
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
	Warrior *warrior,*enemy;
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
	void attacked(Warrior* w);
	void receive(int s);
	void accept();
	
	void print();
	
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
	void cleanWarrior();
	Warrior* judgeBomb();
	
	void print();
};

class Weapon{
public:
	char name[weaponNameLen];
	WeaponType type;
	int power,remain;
public:
	Weapon(WeaponType type_,int power_):type(type_),power(power_),remain(3){
		memcpy(name, weaponName[type],sizeof(name));
	}
	~Weapon(){}
};

class Sword: public Weapon{
public:
	Sword(WeaponType type_,int power_):Weapon(type_,power_) {}
};
class Bomb: public Weapon{
public:
	Bomb(WeaponType type_,int power_):Weapon(type_,power_) {}
};
class Arrow: public Weapon{
public:
	Arrow(WeaponType type_,int power_):Weapon(type_,power_) {}
};

class Warrior{
public:
	Base* belong;
	int location;
	int id;
	WarriorType type;
	
	int kill;
	char identity[identityLen];
	
	vector<Weapon*> weapon;
	
public:
	int add;
	int strength,power;
	
public:
	Warrior(){}
	virtual ~Warrior(){
		for (int i=0;i<weapon.size();++i) delete weapon[i];
	}
	
	void print(){
		cout<<"  warrior: "<<identity;
		cout<<"  strength="<<strength<<"   power="<<power<<"   location="<<location<<"   kill="<<kill<<endl;
	}
	void born(Base* belong_, int location_, int id_, WarriorType type_);
	
	Weapon* newWeapon(int id){
		Weapon *p;
		switch (id){
			case 0:{
				if (power/5 == 0) return NULL;
				p = new Sword(sword, power/5); break;
			}
			case 1: p = new Bomb(bomb, 0); break;
			case 2: p = new Arrow(arrow, arrowPower); break;
		}
		return p;
	}
	
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
		
		opposite->attacked(this);
		
	}
	
	void take(int s);
	
/*	virtual void fightInit(){
		kill=0;
	}
*/
	virtual void attack(Warrior *enemy){
		char msg[mLen];
		sprintf(msg,"%s attacked %s in city %d with %d elements and force %d",
			identity, enemy->identity, location, strength, power);
		msgBox->append(msg);
		
		int p = power;
		Weapon* s = getWeapon(sword);
		if (s) p += s->power;
		enemy->hurt(p);
		if (s){
			s->power = s->power*4 / 5;
			if (s->power==0) deleteWeapon(sword);
		}
	}
	
	int alive(){
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
		
		int p=power/2;
		Weapon* s= getWeapon(sword);
		if (s) p+=s->power;
		enemy->hurt(p);
		if (s){
			s->power = s->power*4 / 5;
			if (s->power == 0) deleteWeapon(sword);
		}
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
	
	void tryShoot(Warrior *enemy){
		if (enemy==0) return ;
		Weapon* w = getWeapon(arrow);
		if (w==0) return ;
		enemy->hurt(arrowPower);
		char msg[mLen];
		if (enemy->alive()){
			sprintf(msg,"%s shot",identity);
		}else
			sprintf(msg,"%s shot and killed %s",identity,enemy->identity);
		msgBox->append(msg);
		w->remain --;
		if (w->remain==0) deleteWeapon(arrow);
	}
	
	Weapon* getWeapon(WeaponType wt){
		for (int i=0;i<weapon.size();++i){
			if (weapon[i]->type == wt) return weapon[i];
		}
		return NULL;
	}
	
	void deleteWeapon(WeaponType wt){
		for (int i=0;i<weapon.size();++i){
			if (weapon[i]->type == wt){
				Weapon* tmp = weapon[i];
				weapon[i]=weapon[weapon.size()-1];
				delete tmp;
			}
		}
		weapon.pop_back();
	}
	
	int tryUseBomb(Warrior* enemy){
		if (getWeapon(bomb)==0) return 0;
		char msg[mLen];
		sprintf(msg,"%s used a bomb and killed %s", identity,enemy->identity);
		msgBox->append(msg);
		return 1;
	}
	
	void report();
	
	virtual int tryRunAway() {return 0;}
};

void Warrior::born(Base* belong_, int location_, int id_, WarriorType type_){
	belong=belong_;
	location=location_;
	id=id_;
	type=type_;
	strength=warriorInfo[type].strength;
	power=warriorInfo[type].power;
	weapon.clear();
	sprintf(identity,"%s %s %d",belong->name, warriorName[type], id);
	
	add=0;
	
	char msg[mLen];
	sprintf(msg, "%s born",identity);
	msgBox->append(msg);
}

class Dragon: public Warrior{
private:
	double morale;
	
public:
	void yell(){
		char msg[mLen];
		sprintf(msg,"%s yelled in city %d",identity,location);
		msgBox->append(msg);
	}
	
	virtual void active_draw(Warrior *enemy){
		morale-=0.2;
	}
	virtual void passive_draw(Warrior *enemy){
		morale-=0.2;
	}
	virtual void active_win(Warrior *enemy){
		morale+=0.2;
	}
	virtual void passive_win(Warrior* enemy){
		morale+=0.2;
	}
	
	virtual void active_finish(){
		if (alive() && morale>0.8) yell();
	}
	
	virtual void init(){
		weapon.clear();
		Weapon* w=newWeapon(id%3);
		if (w) weapon.push_back(w);
		morale= (double)(belong->strength)/(double)warriorInfo[dragon].strength;
		
		char msg[mLen];
		sprintf(msg,"Its morale is %.2f", morale);
		msgBox->noTime();
		msgBox->append(msg);
		msgBox->showTime();
	}
	
};

class Ninja: public Warrior{
public:
	virtual void fightBack(Warrior *enemy){
		// this stupid guy doesn't fight back....
	}
	
	virtual void init(){
		weapon.clear();
		Weapon* w=newWeapon(id%3);
		if (w) weapon.push_back(w);
		w=newWeapon((id+1)%3);
		if (w) weapon.push_back(w);
	}
};

class Iceman: public Warrior{
private:
	int moves;
public:
	virtual void init(){
		moves=0;
		weapon.clear();
		Weapon* w=newWeapon(id%3);
		if (w) weapon.push_back(w);
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
		
		opposite->attacked(this);
	}
};

class Lion: public Warrior{
private:
	int loyalty;
	int pre_strength;
public:
	virtual void init(){
		loyalty=belong->strength;
		char msg[mLen];
		sprintf(msg,"Its loyalty is %d",loyalty);
		msgBox->noTime();
		msgBox->append(msg);
		msgBox->showTime();
	}
	virtual void active_draw(Warrior* enemy){
		loyalty-=loyaltyDecrease;
	}
	virtual void passive_draw(Warrior* enemy){
		loyalty-=loyaltyDecrease;
	}
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
	void runAway(){
		char msg[mLen];
		sprintf(msg,"%s ran away",identity);
		msgBox->append(msg);
	}
	virtual int tryRunAway(){
		if (loyalty<=0){
			runAway();
			return 1;
		}
		return 0;
	}
};

class Wolf: public Warrior{
public:
	void rob(Warrior* enemy){
		for (int i=0;i< enemy->weapon.size();++i){
			WeaponType wt = enemy->weapon[i]->type;
			if (getWeapon(wt)==0)
				weapon.push_back(enemy->weapon[i]);
			else
				delete enemy->weapon[i];
			enemy->weapon[i]=0;
		}
		enemy->weapon.clear();
	}
	virtual void active_win(Warrior* enemy){
		rob(enemy);
	}
	virtual void passive_win(Warrior* enemy){
		rob(enemy);
	}
};

int get(int T){
	int strength,n,t;
	if (scanf("%d%d%d%d%d",&strength,&n,&arrowPower, &loyaltyDecrease, &t)!=5) return 0;
	for (int i=0;i<warriorNumber;++i)
		scanf("%d",&warriorInfo[i].strength);
	for (int i=0;i<warriorNumber;++i)
		scanf("%d",&warriorInfo[i].power);
	printf("Case %d:\n",T);
	
	msgBox = new MessageBox;
	
	BattleField battleField(n,strength);
	
	battleField.run(t);

	return 1;
}

int main(){
	#ifdef local
	freopen("wow_final.in","r",stdin);
	freopen("wow_final.out","w",stdout);
	#endif
	
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

void BattleField::time05(){
	Warrior* tmp = baseRed->warrior;
	if (tmp){
		if (tmp->tryRunAway()){
			delete tmp;
			baseRed->warrior=0;
		}
	}
	for (int i=1;i<=nCity;++i){
		if (city[i]->warriorRed){
			tmp=city[i]->warriorRed;
			if (tmp){
				if (tmp->tryRunAway()){
					delete tmp;
					city[i]->warriorRed=0;
				}
			}
		}
		if (city[i]->warriorBlue){
			tmp=city[i]->warriorBlue;
			if (tmp){
				if (tmp->tryRunAway()){
					delete tmp;
					city[i]->warriorBlue=0;
				}
			}
		}
	}
	tmp=baseBlue->warrior;
	if (tmp){
		if (tmp->tryRunAway()){
			delete tmp;
			baseBlue->warrior=0;
		}
	}
}

void BattleField::time10(){
	vector<Warrior*> tmpRed,tmpBlue;
	
	if (city[1]->warriorBlue){
		city[1]->warriorBlue->reach(baseRed);
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

void BattleField::time35(){
	if (baseRed->warrior && nCity>=1)
		baseRed->warrior->tryShoot( city[1]->warriorBlue );
	for (int i=1;i<=nCity;++i){
		if (i<nCity && city[i]->warriorRed) city[i]->warriorRed->tryShoot(city[i+1]->warriorBlue);
		if (i>1 && city[i]->warriorBlue) city[i]->warriorBlue->tryShoot(city[i-1]->warriorRed);
	}
	if (baseBlue->warrior && nCity>=1)
		baseBlue->warrior->tryShoot( city[nCity]->warriorRed);
}

void BattleField::time38(){
	for (int i=1;i<=nCity;++i){
		Warrior* res = city[i]->judgeBomb();
		if (res){
			int use=0;
			if (res==city[i]->warriorRed)
				use = city[i]->warriorBlue->tryUseBomb(city[i]->warriorRed);
			else
				use = city[i]->warriorRed->tryUseBomb(city[i]->warriorBlue);
			if (use){
				delete city[i]->warriorRed;
				city[i]->warriorRed=0;
				delete city[i]->warriorBlue;
				city[i]->warriorBlue=0;
			}
		}
	}
}

void BattleField::time40(){
	for (int i=1;i<=nCity;++i){
		city[i]->tryFight();
		city[i]->cleanWarrior();
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

void BattleField::time55(){
	if (baseRed->warrior) baseRed->warrior->report();
	for (int i=1;i<=nCity;++i)
		if (city[i]->warriorRed) city[i]->warriorRed->report();
	if (baseBlue->enemy) baseBlue->enemy->report();
	if (baseRed->enemy) baseRed->enemy->report();
	for (int i=1;i<=nCity;++i)
		if (city[i]->warriorBlue) city[i]->warriorBlue->report();
	if (baseBlue->warrior) baseBlue->warrior->report();
}

void BattleField::run(int limit){
	int delta[10]={5,5,10,10,5,3,2,10,5,5};
	int time=0;
	for (int step=0;1; ++step){
		if (time>limit) break;
		msgBox->time = time;
		switch (time%60){
			case 0: time00(); break;
			case 5: time05(); break;
			case 10: time10(); break;
			case 20: time20(); break;
			case 30: time30(); break;
			case 35: time35(); break;
			case 38: time38(); break;
			case 40: time40(); break;
			case 50: time50(); break;
			case 55: time55(); break;
		}
		if (baseRed->taken || baseBlue->taken) break;
		#ifdef debug
		print();
		#endif
		time+= delta[step%10];
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
	enemy=0;
}

Base::~Base(){
	if (warrior) delete warrior;
	if (enemy) delete enemy;
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
	strength-=warriorInfo[warriorType].strength;
	++cnt[warriorType];	
	p->born(this, location, total, warriorType);
	p->init();
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

void Base::attacked(Warrior* w){
	if (enemy) delete enemy;
	enemy=w;
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

void City::cleanWarrior(){
	if (warriorRed && !warriorRed->alive()){
		delete warriorRed;
		warriorRed=0;
	}
	if (warriorBlue && !warriorBlue->alive()){
		delete warriorBlue;
		warriorBlue=0;
	}
}

void City::tryFight(){
	if (curFight!=none) preFight=curFight;
	curFight=none;
	if (warriorRed) warriorRed->kill=0;
	if (warriorBlue) warriorBlue->kill=0;
	if (warriorRed && warriorBlue){
		if (!warriorRed->alive() && !warriorBlue->alive()) return ;

		if (flag==redWin || flag==none && (location%2))
			fight(warriorRed, warriorBlue);
		else
			fight(warriorBlue, warriorRed);
			
		curFight=draw;		
		if (warriorRed->kill) curFight=redWin;
		if (warriorBlue->kill)curFight=blueWin;
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

void City::produce(int s){
	strength+=s;
}

void City::fight(Warrior *a, Warrior *b){
	if (!b->alive()){
		a->kill=1;
		a->active_win(b);
	}else
	if (!a->alive()){
		b->kill=1;
		b->active_win(a);
	}else{
		a->attack(b);
		if (b->alive()){
			b->fightBack(a);
			if (a->alive()){
				a->active_draw(b);
				b->passive_draw(a);
			}else{
				a->lose(b);
				b->kill=1;
				b->passive_win(a);
			}
		}else{
			b->lose(a);
			a->kill=1;
			a->active_win(b);
		}
	}
	a->active_finish();
	b->passive_finish();
}

Warrior* City::judgeBomb(){
	if (warriorRed && warriorRed->alive() && warriorBlue && warriorBlue->alive()){
		Warrior *a, *b;
		if (flag==redWin || (flag==none && location%2==1)) a=warriorRed,b=warriorBlue;
		else a=warriorBlue,b=warriorRed;
		int sa= a->strength, sb= b->strength;
		int pa= a->power;
		if (a-> getWeapon(sword)) pa+=a->getWeapon(sword)->power;
		sb-=pa;
		if (sb>0){
			int pb=b->power/2;
			if (b->getWeapon(sword)) pb+=b->getWeapon(sword)->power;
			if (b->type == ninja) pb=0;
			sa-=pb;
			if (sa<=0) return b;
			return NULL;
		}else
			return a;
	}
	return NULL;
}

void Warrior::take(int s){
	belong->receive(s);
	char msg[mLen];
	sprintf(msg,"%s earned %d elements for his headquarter",identity,s);
	msgBox->append(msg);
}

void Warrior::report(){
	ostringstream os;
	os<< identity<<" has";
	if (weapon.size()==0) os<<" no weapon";
	else{
		int first=1;
		Weapon* tmp;
		tmp=getWeapon(arrow);
		if (tmp){
			os<<' ';
			os<<"arrow("<< tmp->remain << ')';
			first=0;
		}
		tmp=getWeapon(bomb);
		if (tmp){
			if (first) os<<' '; else os<<',';
			os<<"bomb";
			first=0;
		}
		tmp=getWeapon(sword);
		if (tmp){
			if (first) os<<' '; else os<<',';
			os<<"sword("<< tmp->power <<')';
			first=0;
		}
	}
	msgBox->append(os.str().c_str());
}

void BattleField::print(){
	cout<<"BattleField"<<endl;
	baseRed->print();
	for (int i=1;i<=nCity;++i) city[i]->print();
	baseBlue->print();
}
void Base::print(){
	cout<<"Base "<<name<<endl;
	if (warrior) warrior->print();
	if (enemy) enemy->print();
}
void City::print(){
	cout<<"city "<<location<<endl;
	if (warriorRed) warriorRed->print();
	if (warriorBlue)warriorBlue->print();
}