#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<memory.h>
#include<time.h>
#include<conio.h>
#include<windows.h>
using namespace std;
int type[1048576]={0};//棋型對照表
int type2[1048576]={0};
/*
type
 1 連五
 2 活四
 3 活三300
 4 斷三 中290
 5 斷三 旁280
 6 活二50
 7 活二 空一40
 8 活二 空二30
 9 死四400
10 死三 20
*/ 
const int m[10]={1,4,16,64,256,1024,4096,16384,65536,262144};//3進位,4的次方 
const int d[4]={1,24,25,26};//四個方向 位置的索引差 
const int score[11]={0,10000,5000,300,290,280,50,40,30,400,20};
void build1(int now,int number,int key);//遞迴建立棋型對照表
void build2(int now,int number,int key);
void build3(int now,int number,int key);
void build4(int now,int number,int key);
void build5(int now,int number,int space,bool head,int key);
void build6(int now,int number,int space,bool head,int key);
void build7(int now,int number,bool space,int key);
void build8(int now,bool space,int key);
void build9(int now,bool space,int key);
void build10(int now,bool space,int key);
void build11(int now,int number,bool side,int key);
void build12(int now,int number,int key);
void build13(int now,int number,int key);
void build14(int now,int number,bool direct,int key);
void build15(int now,int number,bool direct,int key);
void build_white();//建立白棋棋型表 
void initial();//初始化 
void printing();//畫出棋盤 
void gotoxy(int xpos, int ypos);//設定游標座標 
void SetColor(int f,int b);//設定顏色 
string root;//當前盤面 
int t1,t2;//回傳棋型 
struct node//節點 
{
	int place;
	int value;
	bool visit;
	bool is_evaluated;
	bool terminal;
	int type_sum[11],type2_sum[11];
	int parent;
	vector<int>child;
};
node tree[5000000];//遊戲樹 預留500萬個節點 (原本採用vector實作 但效能明顯較差) 
int last_node=0;//指向最後一個節點的索引值 
void get_type(const string s,const int place,const int i)//取得棋型 
{
	int cnt=0,temp=0;
	for(int p=place-d[i]*5;cnt<5;p+=d[i],cnt++) temp+=s[p]*m[cnt];
	for(int p=place+d[i];cnt<10;p+=d[i],cnt++) temp+=s[p]*m[cnt];
	t1=type[temp];
	t2=type2[temp];
}
node n;
bool update[25*25];//儲存是否更新過 
void genmove(const string s,const int key)//著手生成器 
{
	const int parent=tree[key].parent;//父節點索引 
	const int place=tree[key].place;//上一手位置 
	memset(update,false,sizeof(update));
	update[tree[key].place]=true;
	for(int i=0;i<4;i++)
	{
		for(int p=place-d[i]*5,cnt=0;cnt<5;p+=d[i],cnt++)
		{
			if(s[p]==2)
			{
				update[p]=true;
				for(int j=0;j<4;j++)
				{
					get_type(s,p,j);
					if(t1||t2)  
					{
						n.place=p;
						n.parent=key;
						last_node++;
						tree[last_node]=n;
						tree[key].child.push_back(last_node);
						break;
					}
				}
			}
		}
		for(int p=place+d[i],cnt=5;cnt<10;p+=d[i],cnt++)
		{
			if(s[p]==2)
			{
				update[p]=true;
				for(int j=0;j<4;j++)
				{
					get_type(s,p,j);
					if(t1||t2)
					{
						n.place=p;
						n.parent=key;
						last_node++;
						tree[last_node]=n;
						tree[key].child.push_back(last_node);
						break;
					}
				}
			}
		}
	}
	int now;
	for(int i=0;i<tree[parent].child.size();i++)
	{
		now=tree[parent].child[i];
		if(!update[tree[now].place])
		{
			n.place=tree[now].place;
			n.parent=key;
			last_node++;
			tree[last_node]=n;
			tree[key].child.push_back(last_node);
		}
	}
}
void initial_genmove(const string s,const int key)//著手生成器 
{
	for(int i=5;i<20;i++)
	{
		for(int j=5;j<20;j++)
		{
			if(s[25*i+j]==2)
			{
				for(int k=0;k<4;k++)
				{
					get_type(s,25*i+j,k);
					if(t1||t2)
					{
						n.place=25*i+j;
						n.parent=key;
						last_node++;
						tree[last_node]=n;
						tree[key].child.push_back(last_node);
						break;
					}
				}
			}
		}
	}
}
int type_cnt[11],type2_cnt[11],black,white;
int point(const int key,const bool color)//根據棋型數量統計打分數 
{
	black=0;
	white=0;
	type_cnt[1]=tree[key].type_sum[1];
	type_cnt[2]=tree[key].type_sum[2];
	type2_cnt[1]=tree[key].type2_sum[1];
	type2_cnt[2]=tree[key].type2_sum[2];
	type_cnt[4]=(tree[key].type_sum[4]+tree[key].type_sum[5])/3;
	type2_cnt[4]=(tree[key].type2_sum[4]+tree[key].type2_sum[5])/3;
	type_cnt[3]=tree[key].type_sum[3]/3;
	type_cnt[6]=tree[key].type_sum[6]/2;
	type_cnt[7]=tree[key].type_sum[7]/2;
	type_cnt[8]=tree[key].type_sum[8]/2;
	type_cnt[9]=tree[key].type_sum[9]/4;
	type_cnt[10]=tree[key].type_sum[10]/3;
	type2_cnt[3]=tree[key].type2_sum[3]/3;
	type2_cnt[6]=tree[key].type2_sum[6]/2;
	type2_cnt[7]=tree[key].type2_sum[7]/2;
	type2_cnt[8]=tree[key].type2_sum[8]/2;
	type2_cnt[9]=tree[key].type2_sum[9]/4;
	type2_cnt[10]=tree[key].type2_sum[10]/3;
	if(color)
	{
		if(type_cnt[1]>0) return 9e5;
		if(type2_cnt[1]>0) return -9e5;
		if(type_cnt[2]||type_cnt[9]) return 9e5;
		if((type2_cnt[2]>0||type2_cnt[9]>=2)||(type2_cnt[9]>0&&type2_cnt[3]+type2_cnt[4]>0)) return -9e5;
		if(type_cnt[3]+type_cnt[4]>0) return 8e5;
		if(type2_cnt[3]+type2_cnt[4]>=2) return -8e5;
		for(int i=0;i<11;i++)
		{
			black+=score[i]*type_cnt[i];
			white+=score[i]*type2_cnt[i];
		}
		return 2.5*black-white;
	}
	else
	{
		if(type2_cnt[1]>0) return -9e5;
		if(type_cnt[1]>0) return 9e5;
		if(type2_cnt[2]||type2_cnt[9]) return -9e5;
		if((type_cnt[2]>0||type_cnt[9]>=2)||(type_cnt[9]>0&&type_cnt[3]+type_cnt[4]>0)) return 9e5;
		if(type2_cnt[3]+type2_cnt[4]>0) return -8e5;
		if(type_cnt[3]+type_cnt[4]>=2) return 8e5;
		for(int i=0;i<11;i++)
		{
			black+=score[i]*type_cnt[i];
			white+=score[i]*type2_cnt[i];
		}
		return black-2.5*white;
	}
}
void evaluate(const int key,const string s,const bool color)//評估函數 評估盤面各棋型數量     根據上一手位置周圍以及父節點資訊加速優化 
{
	tree[key].is_evaluated=true;
	string ps=s;//父節點盤面 parent string 
	ps[tree[key].place]=2;
	if(!tree[tree[key].parent].is_evaluated) evaluate(tree[key].parent,ps,!color);
	for(int i=0;i<11;i++)
	{
		tree[key].type_sum[i]=tree[tree[key].parent].type_sum[i];
		tree[key].type2_sum[i]=tree[tree[key].parent].type2_sum[i];
	}
	for(int i=0;i<4;i++)
	{
		get_type(s,tree[key].place,i);
		if(s[tree[key].place]==1) tree[key].type_sum[t1]++;
		else tree[key].type2_sum[t2]++;
		for(int p=tree[key].place-d[i]*5,cnt=0;cnt<5;p+=d[i],cnt++)
		{
			if(s[p]==1)
			{
				get_type(ps,p,i);
				tree[key].type_sum[t1]--;
				get_type(s,p,i);
				tree[key].type_sum[t1]++;
			}
			if(s[p]==0)
			{
				get_type(ps,p,i);
				tree[key].type2_sum[t2]--;
				get_type(s,p,i);
				tree[key].type2_sum[t2]++;
			}
		}
		for(int p=tree[key].place+d[i],cnt=0;cnt<5;p+=d[i],cnt++)
		{
			if(s[p]==1)
			{
				get_type(ps,p,i);
				tree[key].type_sum[t1]--;
				get_type(s,p,i);
				tree[key].type_sum[t1]++;
			}
			if(s[p]==0)
			{
				get_type(ps,p,i);
				tree[key].type2_sum[t2]--;
				get_type(s,p,i);
				tree[key].type2_sum[t2]++;
			}
		}
	}
}
void initial_evaluate(const int key,const string s,const bool color)//評估函數  無父節點資訊 暴力搜索整個棋盤 
{
	tree[key].is_evaluated=true;
	for(int i=5;i<20;i++)
	{
		for(int j=5;j<20;j++)
		{
			if(s[25*i+j]==1)
			{
				for(int k=0;k<4;k++)
				{
					get_type(s,25*i+j,k);
					tree[key].type_sum[t1]++;
				}
			}
			else if(s[25*i+j]==0)
			{
				for(int k=0;k<4;k++)
				{
					get_type(s,25*i+j,k);
					tree[key].type2_sum[t2]++;
				}
			}
		}
	}
}
bool cmp_black(const int a,const int b)//招法排序比較函數 
{
	return tree[a].value>tree[b].value;
}
bool cmp_white(const int a,const int b)
{
	return tree[a].value<tree[b].value;
}
bool check_win(const string s,const int place,const int color)//檢查勝負 
{
	for(int i=0;i<4;i++)
	{
		get_type(root,place,i);
		if(color&&t1==1) return true;
		else if(!color&&t2==1) return true;
	}
	return false;
}
int minimax(string s,int key,bool color,int alpha,int beta,int depth)//minimax搜索+alpha beta剪枝+招法排序提高剪枝率 
{
	if(!tree[key].visit)
	{
		tree[key].visit=true;
		if(check_win(s,tree[key].place,!color))
		{
			tree[key].terminal=true;
			if(color) return tree[key].value=-1e6;
			return tree[key].value=1e6;
		}
	}
	if(tree[key].terminal) return tree[key].value;//葉節點 直接返回 
	if(depth==0) //達搜索深度 評估並返回 
	{
		evaluate(key,s,color);
		return tree[key].value=point(key,color);
	}
	if(tree[key].child.size()==0) genmove(s,key);
	int val,now,i;
	if(color)
	{
		val=-1e8;
		for(i=0;i<tree[key].child.size();i++)
		{
			now=tree[key].child[i];
			s[tree[now].place]=color;
			val=max(val,minimax(s,now,!color,alpha,beta,depth-1));
			s[tree[now].place]=2;
			alpha=max(alpha,val);
			if(alpha>=beta) //beta剪裁 
			{
				i++;
				break;
			}
		}
		stable_sort(tree[key].child.begin(),tree[key].child.begin()+i,cmp_black);
	}
	else
	{
		val=1e8;
		for(i=0;i<tree[key].child.size();i++)
		{
			now=tree[key].child[i];
			s[tree[now].place]=color;
			val=min(val,minimax(s,now,!color,alpha,beta,depth-1));
			s[tree[now].place]=2;
			beta=min(beta,val);
			if(alpha>=beta) //alpha剪裁 
			{
				i++;
				break;
			}
		}
		stable_sort(tree[key].child.begin(),tree[key].child.begin()+i,cmp_white);
	}
	return tree[key].value=val;
}
bool com_color;//電腦持黑或白 
void play_black()
{
	printing();
	com_color=true;
	gotoxy(16,8);
	cout<<"●";
	gotoxy(16,8);
	root[25*12+12]=true;
}
int x=16,y=8;
void starting()
{
	x=16;
	y=8;
	SetColor(7,0);
	system("cls");
	gotoxy(0,0);
	for(int i=0;i<25*25;i++)
	{
		root.push_back(3);
	}
	for(int i=5;i<20;i++)
	{
		for(int k=5;k<20;k++)
		{
			root[25*i+k]=2;
		}
	}
	for(int i=0;i<5000000;i++) tree[i]=(node){0,0,0,0,0,{0},{0},0,vector<int>()};//clear tree
	int key;
	cout<<"玩家先下,按a\n電腦先下,按b\n";
	while(1)
	{
		key=getch();
		if(key==(int)'a'||key==(int)'A') 
		{
			system("cls");
			com_color=false;
			printing();
			break;
		}
		else if(key==(int)'b'||key==(int)'B') 
		{
			system("cls");
			play_black();
			break;
		}
	}
}
int main()
{
	initial();
	int place,key;
	starting();
	while(1) 
	{
		while(1)
		{
			key=getch();
			if(key==224)
			{
				key=getch();
				if(key==72) y--;
				else if(key==80) y++;
				else if(key==75) x-=2;
				else if(key==77) x+=2;
				if(x>30) x=2;//超出邊界 
				else if(x==0) x=30;
				if(y>15) y=1;
				else if(y==0) y=15;
			}
			else if(key==13)
			{
				if(root[25*(y+4)+(x/2+4)]!=2) continue;//非空點 
				if(com_color) cout<<"○";
				else cout<<"●";
				break;
			}
			gotoxy(x,y);
		}
		x/=2;
		x+=4;
		y+=4;
		place=25*y+x;
		if(com_color) root[place]=false;
		else root[place]=true;
		if(check_win(root,place,!com_color)) 
		{
			gotoxy(40,8);
			cout<<"玩家獲勝!!!!!\n";
			gotoxy(40,10);
			cout<<"再下一盤?(Y/N)\n";
			while(1)
			{
				key=getch();
				if(key==(int)'y'||key==(int)'Y') 
				{
					starting();
					break;
				}
				else if(key==(int)'n'||key==(int)'N')
				{
					return 0;
				}
			}
			continue;
		}
		while(last_node!=-1)
		{
			tree[last_node]=(node){0,0,0,0,0,{0},{0},0,vector<int>()};
			last_node--;
		}//clear tree
		tree[0]=(node){place,0,false,true,false,{0},{0},-1,vector<int>()};
		last_node=0;
		initial_genmove(root,0);
		initial_evaluate(0,root,com_color);
		int start=clock();
		gotoxy(0,16);
		for(int i=0;i<7;i++) cout<<"                             \n";
		gotoxy(0,16);
		if(com_color) for(int i=1;i<=4;i++) cout<<"depth:"<<i<<" val="<<minimax(root,0,true,-1e9,1e9,i)<<'\n';
		else for(int i=1;i<=4;i++) cout<<"depth:"<<i<<" val="<<minimax(root,0,false,-1e9,1e9,i)<<'\n';
		/*int now=0,p;
		for(int i=1;i<=5;i++)
		{
			now=tree[now].child[0];
			p=tree[now].place;
			cout<<"第"<<i<<"手:\n";
			cout<<"X:"<<p/25-4<<' ';
			cout<<"y:"<<p%25-4<<endl;
		}*/
		start=clock()-start;
		cout<<"time:"<<start<<"ms\n";
		cout<<"node:"<<last_node+1<<'\n';
		cout<<"電腦正在提心吊膽的等您出招O_O\n";
		place=tree[tree[0].child[0]].place;
		if(com_color) root[place]=true;
		else root[place]=false;
		x=place%25;
		y=place/25;
		x-=4;
		y-=4;
		x*=2;
		gotoxy(x,y);
		if(com_color) cout<<"●";
		else cout<<"○";
		gotoxy(x,y);
		if(check_win(root,place,com_color)) 
		{
			gotoxy(40,8);
			cout<<"電腦獲勝!!!!!\n";
			gotoxy(40,10);
			cout<<"再下一盤?(Y/N)\n";
			while(1)
			{
				key=getch();
				if(key==(int)'y'||key==(int)'Y') 
				{
					starting();
					break;
				}
				else if(key==(int)'n'||key==(int)'N')
				{
					return 0;
				}
			}
			continue;
		}
	}
	return 0;
}
void build1(int now,int number,int key)//連五 
{
	if(now==10) 
	{
		type[key]=1;
		return;
	}
	if(now==number||now==number+5)
	{
		build1(now+1,number,key);
		build1(now+1,number,key+2*m[now]);
		build1(now+1,number,key+3*m[now]);
	}
	else if(number<now&&now<number+5) build1(now+1,number,key+m[now]);
	else
	{
		build1(now+1,number,key);
		build1(now+1,number,key+m[now]);
		build1(now+1,number,key+2*m[now]);
		build1(now+1,number,key+3*m[now]);
	}
}
void build2(int now,int number,int key)//活四 
{
	if(now==10) 
	{
		type[key]=2;
		return;
	}
	if(now==number||now==number+4)
	{
		build2(now+1,number,key+2*m[now]);
	}
	else if(now==number-1||now==number+5)
	{
		build2(now+1,number,key);
		build2(now+1,number,key+2*m[now]);
		build2(now+1,number,key+3*m[now]);
	}
	else if(number<now&&now<number+4)
	{
		build2(now+1,number,key+m[now]);
	}
	else
	{
		build2(now+1,number,key);
		build2(now+1,number,key+m[now]);
		build2(now+1,number,key+2*m[now]);
		build2(now+1,number,key+3*m[now]);
	}
}
void build3(int now,int number,int key)//活三
{
	if(now==10) 
	{
		type[key]=3;
		return;
	}
	if(now==number||now==number+3)
	{
		build3(now+1,number,key+2*m[now]);
	}
	else if(number<now&&now<number+3)
	{
		build3(now+1,number,key+m[now]);
	}
	else
	{
		build3(now+1,number,key);
		build3(now+1,number,key+m[now]);
		build3(now+1,number,key+2*m[now]);
		build3(now+1,number,key+3*m[now]);
	}
} 
void build4(int now,int number,int key)//活二
{
	if(now==10) 
	{
		type[key]=6;
		return;
	}
	if(now==number||now==number+2)
	{
		build4(now+1,number,key+2*m[now]);
	}
	else if(now==number-1||now==number+3)
	{
		build4(now+1,number,key+2*m[now]);
	}
	else if(number<now&&now<number+2)
	{
		build4(now+1,number,key+m[now]);
	}
	else
	{
		build4(now+1,number,key);
		build4(now+1,number,key+m[now]);
		build4(now+1,number,key+2*m[now]);
		build4(now+1,number,key+3*m[now]);
	}
} 
void build5(int now,int number,int space,bool head,int key)//死四 
{
	if(now==10) 
	{
		type[key]=9;
		return ;
	}
	if(now==number)
	{
		if(head)
		{
			build5(now+1,number,space,head,key);
		}
		else
		{
			build5(now+1,number,space,head,key);
			build5(now+1,number,space,head,key+2*m[now]);
			build5(now+1,number,space,head,key+3*m[now]);
		}
	}
	else if(now==number+5)
	{
		if(head)
		{
			build5(now+1,number,space,head,key);
			build5(now+1,number,space,head,key+2*m[now]);
			build5(now+1,number,space,head,key+3*m[now]);
		}
		else
		{
			build5(now+1,number,space,head,key);
		}
	}
	else if(number<now&&now<number+5)
	{
		if(now==number+space)
		{
			build5(now+1,number,space,head,key+2*m[now]);
		}
		else 
		{
			build5(now+1,number,space,head,key+m[now]);
		}
	}
	else
	{
		build5(now+1,number,space,head,key);
		build5(now+1,number,space,head,key+m[now]);
		build5(now+1,number,space,head,key+2*m[now]);
		build5(now+1,number,space,head,key+3*m[now]);
	}
}
void build6(int now,int number,int space,bool head,int key)//死三 
{
	if(now==10) 
	{
		type[key]=10;
		return ;
	}
	if(now==number)
	{
		if(head)
		{
			build6(now+1,number,space,head,key);
		}
		else
		{
			build6(now+1,number,space,head,key+2*m[now]);
		}
	}
	else if(now==number+4)
	{
		if(head)
		{
			build6(now+1,number,space,head,key+2*m[now]);
		}
		else
		{
			build6(now+1,number,space,head,key);
		}
	}
	else if(number<now&&now<number+4)
	{
		if(now==number+space)
		{
			build6(now+1,number,space,head,key+2*m[now]);
		}
		else 
		{
			build6(now+1,number,space,head,key+m[now]);
		}
	}
	else
	{
		build6(now+1,number,space,head,key);
		build6(now+1,number,space,head,key+m[now]);
		build6(now+1,number,space,head,key+2*m[now]);
		build6(now+1,number,space,head,key+3*m[now]);
	}
}
void build7(int now,int number,bool space,int key)//斷三-中 
{
	if(now==10)
	{
		type[key]=4;
		return ;
	}
	if(now==number)
	{
		build7(now+1,number,space,key+m[now]);
	}
	else if(space)
	{
		if(now==number-2)
		{
			build7(now+1,number,space,key+m[now]);
		}
		else if(number-4<now&&now<number+2)
		{
			build7(now+1,number,space,key+2*m[now]);
		}
		else
		{
			build7(now+1,number,space,key);
			build7(now+1,number,space,key+m[now]);
			build7(now+1,number,space,key+2*m[now]);
			build7(now+1,number,space,key+3*m[now]);
		}
	}
	else
	{
		if(now==number+2)
		{
			build7(now+1,number,space,key+m[now]);
		}
		else if(number-2<now&&now<number+4)
		{
			build7(now+1,number,space,key+2*m[now]);
		}
		else
		{
			build7(now+1,number,space,key);
			build7(now+1,number,space,key+m[now]);
			build7(now+1,number,space,key+2*m[now]);
			build7(now+1,number,space,key+3*m[now]);
		}
	}
} 
void build8(int now,bool space,int key)//斷三-旁 
{
	if(now==10)
	{
		type[key]=5;
		return ;
	}
	int a[5];
	if(space)
	{
		a[0]=2;
		a[1]=1;
		a[2]=1;
		a[3]=2;
		a[4]=2;
		if(0<now&&now<6)
		{
			build8(now+1,space,key+a[now-1]*m[now]);
		}
		else
		{
			build8(now+1,space,key);
			build8(now+1,space,key+m[now]);
			build8(now+1,space,key+2*m[now]);
			build8(now+1,space,key+3*m[now]);
		}
	}
	else 
	{
		a[0]=2;
		a[1]=2;
		a[2]=1;
		a[3]=1;
		a[4]=2;
		if(3<now&&now<9)
		{
			build8(now+1,space,key+a[now-4]*m[now]);
		}
		else
		{
			build8(now+1,space,key);
			build8(now+1,space,key+m[now]);
			build8(now+1,space,key+2*m[now]);
			build8(now+1,space,key+3*m[now]);
		}
	}
}
void build9(int now,bool space,int key)//活二-空一
{
	if(now==10)
	{
		type[key]=7;
		return ;	
	}	
	int a[4];
	for(int i=0;i<4;i++) a[i]=2;
	if(space)
	{
		a[1]=1;
		if(1<now&&now<6)
		{
			build9(now+1,space,key+a[now-2]*m[now]);
		}
		else
		{
			build9(now+1,space,key);
			build9(now+1,space,key+m[now]);
			build9(now+1,space,key+2*m[now]);
			build9(now+1,space,key+3*m[now]);
		}
	}
	else
	{
		a[2]=1;
		if(3<now&&now<8)
		{
			build9(now+1,space,key+a[now-4]*m[now]);
		}
		else 
		{
			build9(now+1,space,key);
			build9(now+1,space,key+m[now]);
			build9(now+1,space,key+2*m[now]);
			build9(now+1,space,key+3*m[now]);
		}
	}
} 
void build10(int now,bool space,int key)//活二-空二
{
	if(now==10)
	{
		type[key]=8;
		return ;
	}
	int a[5];
	for(int i=0;i<5;i++) a[i]=2;
	if(space)
	{
		a[1]=1;
		if(0<now&&now<6)
		{
			build10(now+1,space,key+a[now-1]*m[now]);
		}
		else
		{
			build10(now+1,space,key);
			build10(now+1,space,key+m[now]);
			build10(now+1,space,key+2*m[now]);
			build10(now+1,space,key+3*m[now]);
		}
	}
	else
	{
		a[3]=1;
		if(3<now&&now<9)
		{
			build10(now+1,space,key+a[now-4]*m[now]);
		}
		else
		{
			build10(now+1,space,key);
			build10(now+1,space,key+m[now]);
			build10(now+1,space,key+2*m[now]);
			build10(now+1,space,key+3*m[now]);
		}
	}
} 
void build11(int now,int number,bool side,int key)//活四一邊是自己棋子 
{
	if(now==10) 
	{
		type[key]=9;
		return;
	}
	if(now==number||now==number+4)
	{
		build11(now+1,number,side,key+2*m[now]);
	}
	else if(now==number-1)
	{
		if(side)
		{
			build11(now+1,number,side,key+m[now]);
		}
		else
		{
			build11(now+1,number,side,key);
			build11(now+1,number,side,key+2*m[now]);
			build11(now+1,number,side,key+3*m[now]);
		}
	}
	else if(now==number+5)
	{
		if(!side)
		{
			build11(now+1,number,side,key+m[now]);
		}
		else
		{
			build11(now+1,number,side,key);
			build11(now+1,number,side,key+2*m[now]);
			build11(now+1,number,side,key+3*m[now]);
		}
	}
	else if(number<now&&now<number+4)
	{
		build11(now+1,number,side,key+m[now]);
	}
	else
	{
		build11(now+1,number,side,key);
		build11(now+1,number,side,key+m[now]);
		build11(now+1,number,side,key+2*m[now]);
		build11(now+1,number,side,key+3*m[now]);
	}
}
void build12(int now,int number,int key)//活三(OO) 
{
	if(now==10) 
	{
		type[key]=2;
		return;
	}
	if(now==number||now==number+3)
	{
		build12(now+1,number,key+2*m[now]);
	}
	else if(now==number-1||now==number+4)
	{
		build12(now+1,number,key+m[now]);
	}
	else if(now==number-2||now==number+5)
	{
		build12(now+1,number,key);
		build12(now+1,number,key+2*m[now]);
		build12(now+1,number,key+3*m[now]);
	}
	else if(number<now&&now<number+3)
	{
		build12(now+1,number,key+m[now]);
	}
	else
	{
		build12(now+1,number,key);
		build12(now+1,number,key+m[now]);
		build12(now+1,number,key+2*m[now]);
		build12(now+1,number,key+3*m[now]);
	}
}
void build13(int now,int number,int key)//活三(XX) 
{
	if(now==10) 
	{
		type[key]=0;
		return;
	}
	if(now==number||now==number+3)
	{
		build13(now+1,number,key+2*m[now]);
	}
	else if(now==number-1||now==number+4)
	{
		build13(now+1,number,key);
		build13(now+1,number,key+3*m[now]);
	}
	else if(number<now&&now<number+3)
	{
		build13(now+1,number,key+m[now]);
	}
	else
	{
		build13(now+1,number,key);
		build13(now+1,number,key+m[now]);
		build13(now+1,number,key+2*m[now]);
		build13(now+1,number,key+3*m[now]);
	}
}  
void build14(int now,int number,bool direct,int key)//活三(OX&O_)
{
	if(now==10) 
	{
		type[key]=9;
		return;
	}
	if(now==number||now==number+3)
	{
		build14(now+1,number,direct,key+2*m[now]);
	}
	else if(number<now&&now<number+3)
	{
		build14(now+1,number,direct,key+m[now]);
	}
	else if((direct&&now==number-1)||(!direct&&now==number+4))
	{
		build14(now+1,number,direct,key+m[now]);
	}
	else if((direct&&(now==number-2||now==number+4))||(!direct&&(now==number+5||now==number-1)))
	{
		build14(now+1,number,direct,key);
		build14(now+1,number,direct,key+2*m[now]);
		build14(now+1,number,direct,key+3*m[now]);
	}
	else
	{
		build14(now+1,number,direct,key);
		build14(now+1,number,direct,key+m[now]);
		build14(now+1,number,direct,key+2*m[now]);
		build14(now+1,number,direct,key+3*m[now]);
	}
} 
void build15(int now,int number,bool direct,int key)//活二僅一邊是敵子 
{
	if(now==10) 
	{
		type[key]=6;
		return;
	}
	if(now==number||now==number+2)
	{
		build15(now+1,number,direct,key+2*m[now]);
	}
	else if((direct&&now==number-1)||(!direct&&now==number+3))
	{
		build15(now+1,number,direct,key);
		build15(now+1,number,direct,key+3*m[now]);
	}
	else if((direct&&now==number+3)||(!direct&&now==number-1))
	{
		build15(now+1,number,direct,key+2*m[now]);
	}
	else if(number<now&&now<number+2)
	{
		build15(now+1,number,direct,key+m[now]);
	}
	else
	{
		build15(now+1,number,direct,key);
		build15(now+1,number,direct,key+m[now]);
		build15(now+1,number,direct,key+2*m[now]);
		build15(now+1,number,direct,key+3*m[now]);
	}
} 
void build16(int now,int number,int space,int key)//死四擴充 
{
	if(now==10)
	{
		type[key]=9;
		return;
	}
	if(now==number||now==number+6)
	{
		build16(now+1,number,space,key);
		build16(now+1,number,space,key+2*m[now]);
		build16(now+1,number,space,key+3*m[now]);
	}
	else if(now==space) build16(now+1,number,space,key+2*m[now]);
	else if(number<now&&now<number+6) build16(now+1,number,space,key+m[now]);
	else
	{
		build16(now+1,number,space,key);
		build16(now+1,number,space,key+m[now]);
		build16(now+1,number,space,key+2*m[now]);
		build16(now+1,number,space,key+3*m[now]);
	}
}
void build_white()
{
	int s,a,key;
	for(int i=0;i<1048576;i++)
	{
		key=0;
		s=i;
		for(int j=0;j<10;j++)
		{
			a=s%4;
			s=s/4;
			if(a==0)
			{
				a=1;
			}
			else if(a==1)
			{
				a=0;
			}
			key+=a*m[j];
		}
		type2[key]=type[i];
	}
}
void initial()
{
	root.reserve(25*25);
	build10(0,0,0);
	build10(0,1,0);
	build9(0,0,0);
	build9(0,1,0);
	build8(0,0,0);
	build8(0,1,0);
	for(int i=4;i<=5;i++)
	{
		build7(0,i,0,0);
		build7(0,i,1,0);
	}
	for(int i=0;i<=3;i++)
	{
		for(int j=i+1;j<=i+5;j++)
		{
			build16(0,i,j,0);
		}
	}
	for(int i=0;i<5;i++)
	{
		for(int j=1;j<=4;j++)
		{
			build5(0,i,j,0,0);
			build5(0,i,j,1,0);
		}
	}
	for(int i=3;i<5;i++)
	{
		build4(0,i,0);
	}
	for(int i=2;i<5;i++)
	{
		build3(0,i,0);
	}
	for(int i=1;i<5;i++)
	{
		build2(0,i,0);
	}
	for(int i=0;i<5;i++)
	{
		build1(0,i,0);
	}
	for(int i=1;i<5;i++)
	{
		build11(0,i,0,0);
		build11(0,i,1,0);
	}
	for(int i=2;i<5;i++)
	{
		build12(0,i,0);
	}
	for(int i=2;i<5;i++)
	{
		build13(0,i,0);
	}
	for(int i=2;i<5;i++)
	{
		build14(0,i,0,0);
		build14(0,i,1,0);
	}
	for(int i=3;i<5;i++)
	{
		build15(0,i,0,0);
		build15(0,i,1,0);
	}
	build_white();
}
void printing()	
{
	SetColor(0,15);
	cout<<"   A B C D E F G H I J K L M N O";
	gotoxy(40,0);
	cout<<"請用方向建移動 用enter鍵落子\n";
	for(int x=0;x<15;x++)
	{
		if(x<9) cout<<" "<<x+1;
		else cout<<x+1;
		for(int y=0;y<15;y++)
		{
			if(x==0)
			{
				if(0<y&&y<14) {cout<<"┬ ";}
				else if(y==0) {cout<<"┌ ";}
				else if(y==14) {cout<<"┐ ";}
			}
			else if(x==14)
			{
				if(0<y&&y<14) {cout<<"┴ ";}
				else if(y==0) {cout<<"└ ";}
				else if(y==14) {cout<<"┘ ";}
			}
			else if(y==0) {cout<<"├ ";}
			else if(y==14) {cout<<"┤ ";}
			else {cout<<"┼ ";}
		}
		cout<<"\n";
	}
	gotoxy(16,8);
}
void gotoxy(int xpos, int ypos)
{ 
  COORD scrn;
  HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
  scrn.X = xpos; scrn.Y = ypos;
  SetConsoleCursorPosition(hOuput,scrn);
}
void SetColor(int f=7,int b=0)
{
    unsigned short ForeColor=f+16*b;
    HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hCon,ForeColor);
}
