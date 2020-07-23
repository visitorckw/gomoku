#include <iostream>
using namespace std;
int board[19][19]={0};
int line(int x,int y,int d,int a,int p)
{
	if(d==1) x=x-a;
	else if(d==2)
	{
		x=x-a;
		y=y+a;
	}
	else if(d==3) y=y+a;
	else
	{
		x=x+a;
		y=y+a;
	}
	if(0>x||x>18||0>y||y>18) return 2;
	else if(board[x][y]==p) return 1;
	else if(board[x][y]==0) return 0;
	else return 2;
}
int val(int x,int y)
{
	int p=1;
	int a=0,w=0,cut=0,v=0; //a:連子個數 w:被對手堵住 
	int num[5][3][2]={0};//5種 0活1死  是否有斷 
	for(int d=1;d<=4;d++)
	{
		a=0;
		w=0;
		cut=0;
		for(int i=1;;i++)
		{
			if(line(x,y,d,i,1)==p) a=a+1;
			else if(line(x,y,d,i,1)==0)
			{
				if(line(x,y,d,i+1,1)!=1) break;
				else
				{
					cut=cut+1;
					for(int k=1;;k++)
					{
						if(line(x,y,d,i+k,1)==p) a=a+1;
						else if(line(x,y,d,i+k,1)==0) break;
						else
						{
							w=w+1;
							break;
						}
					}
					break;
				}
			} 
			else
			{
				w=w+1;
				break;
			}
		}
		for(int i=-1;;i--)
		{
			if(line(x,y,d,i,1)==p) a=a+1;
			else if(line(x,y,d,i,1)==0) 
			{
				if(line(x,y,d,i-1,1)!=1) break;
				else 
				{
					cut=cut+1;
					for(int k=-1;;k--)
					{
						if(line(x,y,d,i+k,1)==p) a=a+1;
						else if(line(x,y,d,i+k,1)==0) break;
						else
						{
							w=w+1;
							break;
						}
					}
					break;
				}
			}
			else
			{
				w=w+1;
				break;
			}
		}
		if(cut>1) cut=1;
		if(a>4) a=4;
		num[a][w][cut]++;
	}
	v=100*num[1][0][0]+v;
	v=95*num[1][0][1]+v;
	if(num[4][0][0]>0||num[4][1][0]>0||num[4][2][0]>0) v=v+1000000;
	else if(num[3][0][0]>0) v=v+30000;
	else if((num[3][1][0]+num[3][1][1])>=2) v=v+20000;
	else if((num[3][1][0]==1||num[3][1][1]==1)&&(num[2][0][0]==1||num[2][0][1]==1)) v=v+18000;
	else if((num[2][0][0]+num[2][0][1])>=2) v=v+17000;
	else if(num[3][1][0]==1) v=v+8000;
	else if(num[3][1][1]==1) v=v+7999;
	else if(num[2][0][0]==1) v=v+7990;
	else if(num[2][0][1]==1) v=v+7989;//攻擊部分
	int b=0,z=0,cutt=0; //a:連子個數 w:被對手堵住 
	int number[5][3][2]={0};//5種 0活1死  是否有斷 
	for(int d=1;d<=4;d++)
	{
		b=0;
		z=0;
		cutt=0;
		for(int i=1;;i++)
		{
			if(line(x,y,d,i,2)==p) b=b+1;
			else if(line(x,y,d,i,2)==0)
			{
				if(line(x,y,d,i+1,2)!=1) break;
				else
				{
					cutt=cutt+1;
					for(int k=1;;k++)
					{
						if(line(x,y,d,i+k,2)==p) b=b+1;
						else if(line(x,y,d,i+k,2)==0) break;
						else
						{
							z=z+1;
							break;
						}
					}
					break;
				}
			} 
			else
			{
				z=z+1;
				break;
			}
		}
		for(int i=-1;;i--)
		{
			if(line(x,y,d,i,2)==p) b=b+1;
			else if(line(x,y,d,i,2)==0) 
			{
				if(line(x,y,d,i-1,2)!=1) break;
				else
				{
					cutt=cutt+1;
					for(int k=-1;;k--)
					{
						if(line(x,y,d,i+k,2)==p) b=b+1;
						else if(line(x,y,d,i+k,2)==0) break;
						else
						{
							z=z+1;
							break;
						}
					}
					break;
				}
			}
			else
			{
				z=z+1;
				break;
			}
		}
		if(cutt>1) cutt=1;
		if(b>4) b=4;
		number[b][z][cutt]++;
	}
	v=99*number[1][0][0]+v;
	v=94*number[1][0][1]+v;
	if(number[4][0][0]>0||number[4][1][0]>0||number[4][2][0]>0) v=v+500000-50;
	else if(number[3][0][0]>0) v=v+30000-50;
	else if((number[3][1][0]+number[3][1][1])>=2) v=v+20000-50;
	else if((number[3][1][0]==1||number[3][1][1]==1)&&(number[2][0][0]==1||number[2][0][1]==1)) v=v+18000-50;
	else if((number[2][0][0]+number[2][0][1])>=2) v=v+17000-50;
	else if(number[3][1][0]==1) v=v+8000-50;
	else if(number[3][1][1]==1) v=+v+7999-50;
	else if(number[2][0][0]==1) v=v+7990-50;
	else if(number[2][0][1]==1) v=v+7989-50;//防守部分
	return v;
}
int main()
{
	string first;
	cout << "請選擇模式:\n" <<"A:執黑vs電腦\n"<<"B:電腦vs執白\n";
	cin >> first;
	system("cls");
	if(first=="b"||first=="B")
	{
		board[9][9]=1;
		cout << "    01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19\n";
		for(int i=1;i<=19;i++)
		{
			if(i<10) cout << "0"  << i;
			else cout << i;
			for(int k=0;k<19;k++)
			{
				if(board[i-1][k]==1) cout << "  X";
				else if(board[i-1][k]==2) cout << "  O";
				else cout << "  .";
			}
			cout << "\n";
		}
		cout <<"10-10\n";
	}
	else
	{
		cout << "    01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19\n";
		for(int i=1;i<=19;i++)
		{
			if(i<10) cout << "0"  << i;
			else cout << i;
			for(int k=0;k<19;k++)
			{
				if(board[i-1][k]==1) cout << "  X";
				else if(board[i-1][k]==2) cout << "  O";
				else cout << "  .";
			}
			cout << "\n";
		}
	}
	int x,y,mx,my,max=-10000000;
	for(;;)
	{
		cout << "電腦正在提心吊膽的等您出招O_O\n";	
		cout<<"縱坐標:";
		cin >>x;
		cout << "橫坐標:";
		cin >> y; 
		if(board[x-1][y-1]!=0)
		{
			cout << "重複位置判負\n";
			return 0;
		}
		board[x-1][y-1]=2;
		for(int i=0;i<19;i++)
		{
			for(int k=0;k<19;k++)
			{
				if((board[i][k]==0)&&(val(i,k)>=max))
				{
						max=val(i,k);
						mx=i;
						my=k;
				}
			}
		}
		for(int i=0;i<19;i++)
		{
			for(int k=0;k<19;k++)
			{
				if(board[i][k]==2)
				{
					for(int d=1;d<=4;d++)
					{
						int abc=0;
						for(int a=1;;a++)
						{
							if(line(i,k,d,a,2)==1) abc++;
							else break;
						}
						for(int b=-1;;b--)
						{
							if(line(i,k,d,b,2)==1) abc++;
							else break;
						}
						if(abc==4)
						{
							cout << "玩家獲勝!!";
							return 0;
						}
					}
				}
			}
		}
		board[mx][my]=1;
		system("cls");
		cout << "    01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19\n";
		for(int i=1;i<=19;i++)
		{
			if(i<10) cout << "0"  << i;
			else cout << i;
			for(int k=0;k<19;k++)
			{
				if(board[i-1][k]==1) cout << "  X";
				else if(board[i-1][k]==2) cout << "  O";
				else cout << "  .";
			}
			cout << "\n";
		}
		cout << mx+1 << "-" << my+1 << "\n";
		max=-1000000000;
		for(int i=0;i<19;i++)
		{
			for(int k=0;k<19;k++)
			{
				if(board[i][k]==1)
				{
					for(int d=1;d<=4;d++)
					{
						int abcd=0;
						for(int a=1;;a++)
						{
							if(line(i,k,d,a,1)==1) abcd++;
							else break;
						}
						for(int b=-1;;b--)
						{
							if(line(i,k,d,b,1)==1) abcd++;
							else break;
						}
						if(abcd==4)
						{
							cout << "電腦獲勝!!";
							return 0;
						}
					}
				}
			}
		}
	}
	return 0;
}
