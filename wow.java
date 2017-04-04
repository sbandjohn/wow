import java.io.*;
import java.util.*;

class OverAll{
	String warriorName[]={"dragon","ninja","iceman","lion","wolf"};
	int orderRed[]={2,3,4,1,0};
	int orderBlue[]={3,0,1,2,4};
	int warriorEle[]=new int[5];
	int warriorPower[]=new int[5];
	int N,M,R,K,T;
	Msg msg=new Msg();
	void read(Scanner input){
		M=input.nextInt();
		N=input.nextInt();
		R=input.nextInt();
		K=input.nextInt();
		T=input.nextInt();
		for (int i=0;i<5;++i)
			warriorEle[i]=input.nextInt();
		for (int i=0;i<5;++i)
			warriorPower[i]=input.nextInt();
	}
}

class Msg{
	OverAll oa;
	int time;
	String msg;
	void ST(){
		System.out.printf("%03d:%02d ",time/60,time%60);
	}
	void born(Warrior w){
		ST();
		System.out.println(w.identity+" born");
	}
	void dragonInit(Warrior w,double m){
		System.out.printf("Its morale is %.2f\n",m);
	}
	void lionInit(Warrior w,int l){
		System.out.println("Its loyalty is "+l);
	}
	void runAway(Warrior w){
		ST();
		System.out.println(w.identity+" ran away");
	}
	void march(Warrior w,int loc){
		ST();
		System.out.println(w.identity+" marched to city "+loc+" with "+w.element+" elements and force "+w.power);
	}
	void reach(Warrior w, Headquarter h){
		ST();
		System.out.println(w.identity+" reached "+h.name+" with "+w.element+" elements and force "+w.power);
	}
	void shoot(Warrior w,Warrior e){
		ST();
		System.out.print(w.identity+" shot");
		if (!e.alive()) System.out.println(" and killed "+e.identity);
		else System.out.println();
	}
	void bomb(Warrior w,Warrior e){
		ST();
		System.out.println(w.identity+" used a bomb and killed "+e.identity);
	}
	void attack(Warrior w,Warrior e){
		ST();
		System.out.println(w.identity+" attacked "+e.identity+" in city "+w.loc+" with "+w.element+" elements and force "+w.power);
	}
	void fightBack(Warrior w,Warrior e){
		ST();
		System.out.println(w.identity+" fought back against "+e.identity+" in city "+w.loc);
	}
	void killed(Warrior w){
		ST();
		System.out.println(w.identity+" was killed in city "+w.loc);
	}
	void yell(Warrior w){
		ST();
		System.out.println(w.identity+" yelled in city "+w.loc);
	}
	void take(Warrior w,int ele){
		ST();
		System.out.println(w.identity+" earned "+ele+" elements for his headquarter");
	}
	void raise(City c,String color){
		ST();
		System.out.println(color+" flag raised in city "+c.loc);
	}
	void taken(Headquarter h){
		ST();
		System.out.println(h.name+" was taken");
	}
	void report(Headquarter h){
		ST();
		System.out.println(""+h.element+" elements in "+h.name);
	}
	void report(Warrior w){
		ST();
		System.out.print(w.identity+" has ");
		boolean first=true;
		if (w.arrow!=null){
			first=false;
			System.out.printf("arrow(%d)",w.arrow.remain);
		}
		if (w.bomb!=null){
			if (!first) System.out.print(',');
			first=false;
			System.out.print("bomb");
		}
		if (w.sword!=null){
			if (!first) System.out.print(',');
			first=false;
			System.out.printf("sword(%d)",w.sword.power);
		}
		if (first) System.out.print("no weapon");
		System.out.println();
	}
}

class BattleField{
	OverAll oa;
	Headquarter hRed,hBlue;
	City city[];
	int nCity;
	
	BattleField(OverAll oa_){
		oa=oa_;
		hRed=new Headquarter(oa, "red", oa.orderRed,oa.M);
		hBlue=new Headquarter(oa,"blue",oa.orderBlue,oa.M);
		nCity=oa.N;
		city=new City[oa.N+1];
		for (int i=1;i<=nCity;++i) city[i]=new City(oa, i);
	}
	
	void time00(){
		hRed.makeWarrior();
		hBlue.makeWarrior();
	}
	void time05(){
		if (hRed.warrior!=null)
			if (hRed.warrior.tryRun()) hRed.warrior=null;
		for (int i=1;i<=nCity;++i){
			if (city[i].wRed!=null)
				if (city[i].wRed.tryRun()) city[i].wRed=null;
			if (city[i].wBlue!=null)
				if (city[i].wBlue.tryRun()) city[i].wBlue=null;
		}
		if (hBlue.warrior!=null)
			if (hBlue.warrior.tryRun()) hBlue.warrior=null;
	}
	void time10(){
		Warrior tmp[] = new Warrior[2*nCity+2];
		int tot=0;
		
		if (city[1].wBlue!=null){
			city[1].wBlue.reach(hRed);
		}
		if (hRed.warrior!=null){
			hRed.warrior.march(1);
			tmp[tot++]=hRed.warrior;
		}
		for (int i=1;i<=nCity;++i){
			if (i>1 && city[i-1].wRed!=null){
				city[i-1].wRed.march(i);
				tmp[tot++]=city[i-1].wRed;
			}
			if (i<nCity && city[i+1].wBlue!=null){
				city[i+1].wBlue.march(i);
				tmp[tot++]=city[i+1].wBlue;
			}
		}
		if (hBlue.warrior!=null){
			hBlue.warrior.march(nCity);
			tmp[tot++]=hBlue.warrior;
		}
		if (city[nCity].wRed!=null){
			city[nCity].wRed.reach(hBlue);
		}
		hRed.warrior=hBlue.warrior=null;
		for (int i=1;i<=nCity;++i) city[i].wRed=city[i].wBlue=null;
		for (int i=0;i<tot;++i){
			if (tmp[i].color=="red") city[tmp[i].loc].wRed=tmp[i];
			if (tmp[i].color=="blue")city[tmp[i].loc].wBlue=tmp[i];
		}
	}
	void time20(){
		for (int i=1;i<=nCity;++i) city[i].element+=10;
	}
	void time30(){
		for (int i=1;i<=nCity;++i) city[i].tryTake();
		hRed.accept();
		hBlue.accept();
	}
	void time35(){
		if (hRed.warrior!=null) hRed.warrior.tryShoot(city[1].wBlue);
		for (int i=1;i<=nCity;++i){
			if (i!=nCity && city[i].wRed!=null) city[i].wRed.tryShoot(city[i+1].wBlue);
			if (i!=1 && city[i].wBlue!=null) city[i].wBlue.tryShoot(city[i-1].wRed);
		}
		if (hBlue.warrior!=null) hBlue.warrior.tryShoot(city[nCity].wRed);
	}
	void time38(){
		for (int i=1;i<=nCity;++i) city[i].tryBomb();
	}
	void time40(){
		for (int i=1;i<=nCity;++i){
			city[i].tryFight();
			city[i].clean();
			city[i].tryTake();
			city[i].tryRaise();
		}
		for (int i=nCity;i>=1;--i)
			if (city[i].wRed!=null && city[i].wRed.win)
				hRed.award(city[i].wRed);
		for (int i=1;i<=nCity;++i)
			if (city[i].wBlue!=null && city[i].wBlue.win)
				hBlue.award(city[i].wBlue);
		hRed.accept();
		hBlue.accept();
	}
	void time50(){ hRed.report();hBlue.report(); }
	void time55(){
		if (hRed.warrior!=null) hRed.warrior.report();
		for (int i=1;i<=nCity;++i)
			if (city[i].wRed!=null) city[i].wRed.report();
		if (hBlue.enemy!=null) hBlue.enemy.report();
		if (hRed.enemy!=null) hRed.enemy.report();
		for (int i=1;i<=nCity;++i)
			if (city[i].wBlue!=null) city[i].wBlue.report();
		if (hBlue.warrior!=null) hBlue.warrior.report();
	}
	
	void run(){
		int delta[]={5,5,10,10,5,3,2,10,5,5};
		int time=0;
		int tick=0;
		while (time<=oa.T){
			oa.msg.time=time;
			switch (time%60){
				case   0: time00(); break;
				case   5: time05(); break;
				case  10: time10(); break;
				case  20: time20(); break;
				case  30: time30(); break;
				case  35: time35(); break;
				case  38: time38(); break;
				case  40: time40(); break;
				case  50: time50(); break;
				case  55: time55(); break;
			}
			if (hRed.taken() || hBlue.taken()) break;
			time+=delta[tick%10];
			tick++;
		}
	}
}

class Headquarter{
	String color;
	String name;
	OverAll oa;
	Warrior warrior,enemy;
	int element,add;
	int nEnemy;
	int order[];
	int cur,tot;
	
	Headquarter(OverAll oa_,String color_, int[] order_,int element_){
		oa=oa_;
		color=color_;
		name=color+" headquarter";
		order=order_;
		cur=0;
		nEnemy=0;
		warrior=enemy=null;
		element=element_;
		add=0;
		tot=0;
	}
	
	void makeWarrior(){
		int now=order[cur];
		if (element< oa.warriorEle[now]) return ;
		Warrior w=null;
		switch (oa.warriorName[now]){
			case "dragon": w= new Dragon(); break;
			case "ninja" : w= new Ninja();  break;
			case "iceman": w= new Iceman(); break;
			case "lion"  : w= new Lion();   break;
			case "wolf"  : w= new Wolf();   break;
		}
		tot++;
		element-=oa.warriorEle[now];
		w.born(oa, this, tot, oa.warriorName[now], oa.warriorEle[now], oa.warriorPower[now]);
		w.init();
		warrior=w;
		cur=(cur+1)%5;
	}
	
	void attacked(Warrior e){
		enemy=e;
		nEnemy++;
		if (taken()) oa.msg.taken(this);
	}
	boolean taken(){
		return nEnemy==2;
	}
	void report(){ oa.msg.report(this); }
	void accept(){
		element+=add;
		add=0;
	}
	void award(Warrior w){
		if (w==null) return ;
		if (element>=8){
			w.element+=8;
			element-=8;
		}
	}
}

class City{
	OverAll oa;
	Warrior wRed,wBlue;
	String flag,preFight,curFight;
	int element;
	int loc;
	
	City(OverAll oa_,int loc_){
		oa=oa_;
		loc=loc_;
		wRed=wBlue=null;
		flag="none";
		preFight=curFight="none";
		element=0;
	}
	
	void tryBomb(){
		if (wRed==null || wBlue==null) return ;
		if (!wRed.alive() || !wBlue.alive()) return ;
		Warrior active,passive,winner,loser;
		if (flag=="red" || flag=="none" && loc%2==1){
			active=wRed; passive=wBlue;
		}else{
			active=wBlue; passive=wRed;
		}
		int pe= passive.element - active.getAttack();
		if (pe<=0) winner=active;
		else{
			int ae=active.element-passive.getFightBack();
			if (ae<=0) winner=passive;
			else winner=null;
		}
		if (winner==null) return ;
		if (winner==wRed) loser=wBlue;
		else loser=wRed;
		if (loser.bomb!=null){
			oa.msg.bomb(loser,winner);
			wRed=wBlue=null;
		}
	}
	
	Warrior fight(Warrior active, Warrior passive){
		Warrior w;
		if (!active.alive()){
			passive.passive_win(active);
			w=passive;
		}else
		if (!passive.alive()){
			active.active_win(passive);
			w=active;
		}else{
			active.attack(passive);
			if (passive.alive()){
				passive.fightBack(active);
				if (active.alive()){
					active.active_draw(passive);
					passive.passive_draw(active);
					w=null;
				}else{
					active.loss(passive);
					passive.passive_win(active);
					w=passive;
				}
			}else{
				passive.loss(active);
				active.active_win(passive);
				w=active;
			}
		}
		active.active_finish(passive);
		passive.passive_finish(active);
		return w;
	}
	void tryFight(){
		if (curFight!="none"){
			preFight=curFight;
			curFight="none";
		}
		if (wRed!=null) wRed.win=false;
		if (wBlue!=null) wBlue.win=false;
		if (wRed==null || wBlue==null) return ;
		if (!wRed.alive() && !wBlue.alive()) return ;
		Warrior active,passive,winner;
		if (flag=="red" || flag=="none" && loc%2==1){
			active=wRed; passive=wBlue;
		}else{
			active=wBlue; passive=wRed;
		}
		winner=fight(active,passive);
		if (winner==null){ curFight="draw";return ;}
		winner.win=true;
		curFight=winner.color;
	}
	void clean(){
		if (wRed!=null && !wRed.alive()) wRed=null;
		if (wBlue!=null && !wBlue.alive()) wBlue=null;
	}
	void tryTake(){
		if (element==0) return ;
		if (wRed==null && wBlue==null) return ;
		if (wRed!=null && wBlue!=null) return ;
		if (wRed!=null && wBlue==null) wRed.take(element);
		if (wRed==null && wBlue!=null) wBlue.take(element);
		element=0;
	}
	void tryRaise(){
		if (curFight==preFight && curFight!="none" && curFight!="draw"){
			if (curFight!=flag){
				flag=curFight;
				oa.msg.raise(this, flag);
			}
		}
	}
}

class Warrior{
	OverAll oa;
	int element,power,preElement;
	Weapon sword,bomb,arrow;
	String name,color,identity;
	Headquarter belong;
	int loc;
	boolean win;
	int id;
	
	Warrior(){}
	void born(OverAll oa_, Headquarter h, int id_, String name_,int element_, int power_){
		oa=oa_;
		belong=h;
		color=h.color;
		name=name_;
		id=id_;
		identity=color+" "+name+" "+id_;
		element=element_;
		power=power_;
		sword=bomb=arrow=null;
		oa.msg.born(this);
	}
	void init(){}
	boolean alive(){ return element>0; }
	boolean tryRun(){return false;}
	void march(int to){
		loc=to;
		oa.msg.march(this, to);
	}
	void reach(Headquarter h){
		oa.msg.reach(this,h);
		h.attacked(this);
	}
	void take(int ele){
		belong.add+=ele;
		oa.msg.take(this,ele);
	}
	boolean tryShoot(Warrior e){
		if (arrow==null || e==null) return false;
		e.hurt(arrow.power);
		arrow.use();
		if (arrow.remain==0) arrow=null;
		oa.msg.shoot(this,e);
		return true;
	}
	int getAttack(){
		if (sword!=null) return sword.power+power;
		else return power;
	}
	int getFightBack(){
		if (sword!=null) return power/2 + sword.power;
		else return power/2;
	}
	
	void attack(Warrior e){
		e.hurt(getAttack());
		if (sword!=null){
			sword.use();
			if (sword.power<=0) sword=null;
		}
		oa.msg.attack(this,e);
	}
	void hurt(int e){
		preElement=element;
		element-=e;
	}
	void fightBack(Warrior e){
		e.hurt(getFightBack());
		if (sword!=null){
			sword.use();
			if (sword.power<=0) sword=null;
		}
		oa.msg.fightBack(this,e);
	}
	
	void active_win(Warrior e){}
	void active_draw(Warrior e){}
	void passive_win(Warrior e){}
	void passive_draw(Warrior e){}
	void active_finish(Warrior e){}
	void passive_finish(Warrior e){}
	void loss(Warrior e){
		oa.msg.killed(this);
	}
	
	void getWeapon(int i){
		switch (i){
			case 0:
				sword=new Sword(oa, power/5);
				if (sword.power==0) sword=null;
				break;
			case 1:
				bomb=new Bomb(oa);
				break;
			case 2:
				arrow=new Arrow(oa, oa.R);
				break;
		}
	}
	
	void report(){ oa.msg.report(this);}
}

class Dragon extends Warrior{
	double morale;
	void init(){
		getWeapon(id%3);
		morale=(double)belong.element/(double)oa.warriorEle[0];
		oa.msg.dragonInit(this,morale);
	}
	void active_finish(Warrior e){
		if (alive() && morale>0.8)
			oa.msg.yell(this);
	}
	void active_win(Warrior e){ morale+=0.2; }
	void passive_win(Warrior e){ morale+=0.2; }
	void active_draw(Warrior e){ morale-=0.2; }
	void passive_draw(Warrior e){ morale-=0.2; }
}
class Ninja extends Warrior{
	void init(){
		getWeapon(id%3);
		getWeapon((id+1)%3);
	}
	void fightBack(Warrior e){
		// this stupid guy doesn't fight back
	}
	int getFightBack(){return 0;}
}
class Iceman extends Warrior{
	int steps;
	void init(){
		steps=0;
		getWeapon(id%3);
	}
	void march(int to){
		steps++;
		if (steps%2==0){
			element-=9;
			if (element<=0) element=1;
			power+=20;
		}
		loc=to;	
		oa.msg.march(this,to);
	}
	void reach(Headquarter h){
		steps++;
		if (steps%2==0){
			element-=9;
			if (element<=0) element=1;
			power+=20;
		}
		oa.msg.reach(this,h);
		h.attacked(this);
	}
}
class Lion extends Warrior{
	int loyalty;
	void init(){
		loyalty=belong.element;
		oa.msg.lionInit(this,loyalty);
	}
	void active_draw(Warrior e){ loyalty-=oa.K; }
	void passive_draw(Warrior e){ loyalty-=oa.K; }
	void loss(Warrior e){
		e.element+=preElement;
		oa.msg.killed(this);
	}
	boolean tryRun(){
		if (loyalty<=0){
			oa.msg.runAway(this);
			return true;
		}else return false;
	}
}
class Wolf extends Warrior{
	void rob(Warrior e){
		if (sword==null && e.sword!=null){
			sword=e.sword; e.sword=null;
		}
		if (bomb==null  && e.bomb!=null){
			bomb=e.bomb; e.bomb=null;
		}
		if (arrow==null && e.arrow!=null){
			arrow=e.arrow; e.arrow=null;
		}
	}
	void active_win(Warrior e){ rob(e); }
	void passive_win(Warrior e){ rob(e); }
}

class Weapon{
	OverAll oa;
	String name;
	int power,remain;
	void use(){}
	Weapon(OverAll oa_, String name_,int power_){
		oa=oa;
		power=power_;
		name=name_;
		remain=3;
	}
}

class Sword extends Weapon{
	Sword(OverAll oa_, int power_){ super(oa_,"sword",power_); }
	void use(){ power=power*4/5; }
}
class Bomb  extends Weapon{
	Bomb(OverAll oa_){super(oa_,"bomb",0);}
}
class Arrow extends Weapon{
	Arrow(OverAll oa_, int power_){ super(oa_,"arrow",power_);}
	void use(){ remain--; }
}

class Main{
	public static void main(String args[]){
		// this is comment
		Scanner input = new Scanner(System.in);
		int nCase;
		nCase=input.nextInt();
		OverAll oa=new OverAll();
		for (int i=1;i<=nCase;++i){
			System.out.println("Case "+i+':');
			oa.read(input);
			BattleField battleField=new BattleField(oa);
			battleField.run();
		}
	}
}
