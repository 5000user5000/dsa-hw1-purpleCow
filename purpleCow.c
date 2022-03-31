#include<stdio.h>
#include<stdlib.h>
#include <string.h> // strtok


//人 
typedef struct usb
{
    struct usb *pre;
    struct usb *nxt;
    int group;
    int id;
    int tag;//最好用其他的node來記憶rev
	struct Toilet *Rev;//居然可以 
};

//廁所
typedef struct 
{
    struct usb *head;
    struct usb *tail;
    struct Toilet *nxtRev;//用來記下一個reverse 
    int status;//close or not,可用bool取代 
}Toilet;//名字擺後面,之後呼叫不用像order那樣每次都要加struct,也可上面宣告一名,下面又宣告不同名(之後都使用此) 

//指令 
typedef struct Order
{
	char type;//儲存指令的第一字母來判斷enter close leave go
	int num[4];//最多會有3個指令,多用一個保險
	struct Order *nxt;//找下一個 
	//struct Order *top;//最上面,方便找 ,似乎沒用 
}; 

void leave(Toilet* toilet,int m);
void go(Toilet* toilet,int m);
void enter(Toilet* toilet,int id,int group,int m,struct usb* person);
void close(Toilet* toilet,Toilet* revtoilet,int m,int M);
void printout(Toilet* toilet,int m);
void command(char type,Toilet* toilet,struct Order* order,Toilet* revtoilet,int M); 
struct usb* nxtorpre(struct usb* person);//確認是否為反轉,給下一個的地址 
void assignNxtorpre(struct usb* person);//賦值給下一個,i.e.移動到下一個
struct usb* printRev(struct usb* person); //印出rev並回傳當前地址 



int main()
{
  int m,n,k;//廁所數 指令數 團體數 
  char str[40];//每行指令 40夠用 
  const char* d = "  ";
  char *p;
  struct Order* order;
  //struct Order* orderTop;
  Toilet *toilet,*revtoilet;//revtoilet來記錄反向的人 
  int w;
  
  
   order = malloc(sizeof(struct Order));//先宣告一個就好,之後再動態增加,這樣之後就能動態free掉 
   revtoilet = malloc(sizeof(Toilet));//初始化.這樣沒malloc的pre nxt都會指向null 
   order = calloc(1,sizeof(struct Order));
   revtoilet = calloc(1,sizeof(Toilet));
   
   struct Order *top = malloc(sizeof(struct Order)); ;//存開頭order的位置 
   top = calloc(1,sizeof(struct Order));//要使用就calloc,不然會出錯 
   top = order;
 
  //輸入 
  scanf("%d",&m);//忘記加& 
  scanf("%d",&n);
  scanf("%d",&k);
  

  
  toilet= malloc(sizeof(Toilet)*m);//動態規劃
  toilet= calloc(m,sizeof(Toilet));
  fgets(str,40,stdin);//先放一個在前面,因為第一個只有\n 
  
  for(int i=0;i<n;i++)//一個個輸入指令
  {
    
	fgets(str,40,stdin);//不能用scanf(因為空格就不讀取,只能fget) 
	w=0;
	//memset(str, 0, 40);//用前先清空 
	
    
	p = strtok(str, d); //把輸入分開 指令和數字以空格隔開 
    order->type = p[0];//get指令種類 
    p = strtok(NULL, d);
    
	while (p != NULL) { //把數字都輸入到order上 
       // printf("這次的p = %s\n", p);
        order->num[w++]=atoi(p);//存數字,用內建的string->int ,本想用自己的,但是最後一個數總會亂掉 
        p = strtok(NULL, d);		   
    }
    if(i != n-1)//最後一次就不用,這樣後面才會指向null 
    {
    	order->nxt = malloc(sizeof(struct Order));//給下一個空間
        order->nxt = calloc(1,sizeof(struct Order));
	    order = order->nxt;
    	
	}
    
	
    
  } 
  
  
  while(top!=NULL)
  {
  	//printf("command++ \n");
  	command(top->type,toilet,top,revtoilet,m);//開始執行指令 
  	
  	top = top->nxt;
  }
 
 //printf("print");
 printout(toilet,m);//印出結果 
 
 
 /*while(top!=NULL) //輸出linked list看看 
 {
 	printf("%c ",top->type);
 	for(int q=0;q<3;q++)
    {
  	 printf("%d ",top->num[q]);
    }
    printf("\n");
    top = top->nxt;
 }*/
 
  
  
  free(order);
  free(p);
  free(toilet);
  free(revtoilet);
  free(top) ;
  
  exit(0);//較快 

  return 0; 
}

void command(char type,Toilet* toilet,struct Order* order,Toilet* revtoilet,int M) 
{
	int com1,com2,com3;//指令1~3 
	switch(type)
	{
	
		case 'e':
			com1 = order->num[0];
			com2 = order->num[1];
			com3 = order->num[2];
			struct usb* person = malloc(sizeof(struct usb));//這裡宣布的能用嗎? nxt pre沒malloc 
			person = calloc(1,sizeof(struct usb));
			person->id = com2;
			person->group =com1;
			//printf("enter++");//test use
			enter(toilet,com2,com1,com3,person);
			break;
		case 'l':
			com1 = order->num[0];
			leave(toilet,com1);
			break;
		case 'g':
			com1 = order->num[0];
			go(toilet,com1);
			break;
		case 'c':
			com1 = order->num[0];
			close(toilet,revtoilet,com1,M);
			break;
		default:
			break;
	
	}
}

void leave(Toilet* toilet,int m)//之後補tag rev.. 
{
	Toilet* newtoilet = toilet +m;//指向第m廁所address
	if(newtoilet->tail->Rev== NULL)
	{
	  newtoilet->tail =newtoilet->tail->pre;//把上一個連到廁所的尾巴
	  newtoilet->tail->nxt = NULL;//把尾巴的下一個去掉 
	
	} 
    else {
    	Toilet* revtoilet = newtoilet->tail->Rev;//先把rev收起來 
		newtoilet->tail =newtoilet->tail->nxt;//倒過來
		//newtoilet = newtoilet->tail;newtoilet->tail應該是usb型 非toilet型 
		
		//傳承rev 
		if(revtoilet->head == revtoilet->tail)
		{
		  //do nothing,最後一位就不會再傳承了  
		}
		else
		{
		 revtoilet->tail = newtoilet->tail;
		 newtoilet->tail->Rev =  revtoilet;//雙方互相紀錄,here modify(toilet should not use usb assign)
		
		} 
		//printf("leave id = %d \n",newtoilet->tail->id);//to check
		//printf("leave pre id = %d \n",newtoilet->tail->pre->id);//to check
    	newtoilet->tail->pre = NULL;//把尾巴的下一個去掉 
	}

	//要free? 應該不用 ,區域變數 
	 
}
void go(Toilet* toilet,int m)
{
	Toilet* newtoilet = toilet +m;//指向第m廁所address
	//newtoilet = newtoilet->head; 人和廁所不同類 
	//assignNxtorpre(newtoilet->head);//取代下面的指令 
	//newtoilet->head = newtoilet->head->nxt;//......如果沒有nxt會錯誤嗎?
	if(newtoilet->head->Rev !=NULL)//如果原開頭是rev 
	{
		Toilet* revtoilet = newtoilet->head->Rev;
		if(revtoilet->head == revtoilet->tail){//如果rev對head和tail指定相同->最後一個rev 
			//do nothing,最後一位就不會再傳承了 
		}
		else
		{
		  revtoilet->head = newtoilet->head->pre;//把rev傳給下面一位,不能 newtoilet->head->Rev->head...箭頭不接太多
		  newtoilet->head->pre->Rev = revtoilet;//互相紀錄,這行是把rev傳下去 
		} 
		//傳承完後,換接頭 
		newtoilet->head =  newtoilet->head->pre;//
		newtoilet->head->nxt = newtoilet;//雙向 nxt not pre
	} 
	else
	{
	  newtoilet->head =  newtoilet->head->nxt;
	  if(newtoilet->head->Rev !=NULL)newtoilet->head->nxt = newtoilet;//頭的上一個是指向廁所 
	  else newtoilet->head->pre = newtoilet;//如果下一個是rev	  
    } 
    //printf("head = %d \n",newtoilet->head->id);
    //printf("head pre = %d \n",newtoilet->head->pre->id);
}
void enter(Toilet* toilet,int id,int group,int m,struct usb* person)
{
	Toilet* newtoilet = toilet +m; 
	if(newtoilet->head ==NULL)
	{
		newtoilet->head = malloc(sizeof(struct Order));
		newtoilet->head = calloc(1,sizeof(struct Order));//應該可省 
		newtoilet->head = person;
		newtoilet->tail = malloc(sizeof(struct Order));//忘了補上 
		newtoilet->tail =  person;
		return ;//加快結束 
		
	}
	
	else
	{
	  struct usb* others= malloc(sizeof(struct usb));
	  //others= calloc(1,sizeof(struct usb));
	  others= newtoilet->head ;//用來移動
	  
	  while(nxtorpre(others)!=NULL&&others->group != person->group )//也可改用others id等來判定 ,還要補rev //->nxt
	  {
	  	if(others->Rev==NULL){
	  		person->pre = others;
	  	    others = others->nxt;
		  }
		else
		{
			person->nxt = others;//前面 
	  	    others = others->pre;
		}
	  	
	  }
	  //while中止2caces,1是到null(將此位置指定給person) 
	  if(nxtorpre(others)==NULL)//沒考慮到5(0) 8(1) 7(0) 這種在8的時候,符合tag和next的問題,但是這裡錯誤 //->nxt
	  {
	  	person->pre = others;//
	  	//others->nxt = malloc(sizeof(struct usb));
	  	//others->nxt = calloc(1,sizeof(struct usb));
	  	if(others->Rev==NULL)others->nxt = person;//null = person,這裡的null非other->nxt 
	  	else others->pre = person; 
	  	newtoilet->tail = person;//tail別忘補上 
	  }
	  else//2是如果遇到同id的 
	  {
	  	while(nxtorpre(others) !=NULL&&others->group == person->group)//條件改成同id ,null要先擺要前面,因為null沒法指向group 
	  	{
	  	  //person->pre = malloc(sizeof(struct usb)); 
	  	  //person->pre = calloc(1,sizeof(struct usb));
		  if(others->Rev==NULL){
	  		person->pre = others;
	  	    others = others->nxt;
		  }
	     else
		  {
			person->pre = others;//前面,person是正常的,不用改 
	  	    others = others->pre;
		   }
		} 
	    
	    if(others->group != person->group)//皆在其前面 
	    {
		   //others->pre = person->pre;
		   
		   if(others->Rev==NULL &&others->pre->Rev==NULL)others->pre->nxt = person;//一般型 && others->pre!=NULL
		   else if(others->Rev!=NULL &&others->pre->Rev!=NULL)others->nxt->pre = person;
		   else if(others->Rev==NULL &&others->pre->Rev!=NULL)others->pre->pre = person;
		   else if(others->Rev!=NULL &&others->pre->Rev==NULL)others->nxt->nxt = person;
		   //person->nxt =  malloc(sizeof(struct usb));
		   //person->pre =  malloc(sizeof(struct usb));
		   person->nxt = others ;//不能把null指給別人
		   if(others->Rev==NULL)others->pre = person ; 
		   else others->nxt = person ; 
		   
	    	
		}
		else // others->nxt ==NULL
	    {
	    	person->pre = others;
	     	if(others->Rev==NULL)others->nxt = person;
	     	else others->pre = person;
	    	newtoilet->tail = person;//tail別忘補上 
	    }
	  }
	}
	
 } 
void close(Toilet* toilet,Toilet* revtoilet,int m,int M)//m是指定的位置,M是廁所總數 
{
  Toilet* oldtoilet = toilet+m;//舊的廁所 
  Toilet* newtoilet = toilet+m;//新的 
  newtoilet->status = -1;//表示關閉
  while(newtoilet != NULL&& newtoilet->status == -1) //當又遇到空的且非超過第一個廁所(可能有問題) 
  {
     newtoilet--; //往前找 
  } 
  //2可能 1是前面都關了 2是找到有
  if(newtoilet == NULL)//前面都關
  {
     newtoilet = toilet;//重新指回
     if(m=M)//已經是最後一個
	 {
	   free(newtoilet->head);//把這排free 
	   free(newtoilet->tail);//把這排free 
	 } 
	 else
	 {
	 	newtoilet++;
		 while(newtoilet != NULL && newtoilet->status ==-1)//原是用newtoilet == toilet+M 
		 {newtoilet++;}//往後找 
		 if(newtoilet != NULL)//還有位置的話 ,且是open 
		 {
		   //revtoilet->head = oldtoilet->tail;
		   //revtoilet->tail = oldtoilet->head;
		   //先反轉 
		   revtoilet->head = malloc(sizeof(struct usb));
	       revtoilet->tail= malloc(sizeof(struct usb));
		   oldtoilet->tail->Rev = revtoilet;// ->head 人只要分到rev就好,tail head後面再分配 
		   oldtoilet->head->Rev = revtoilet;// ->tail
		   revtoilet->head = oldtoilet->tail;//只要rev知道誰是頭是尾就好 
		   revtoilet->tail = oldtoilet->head;
		   
		   newtoilet->tail->nxt = malloc(sizeof(struct usb));//先malloc,可能可省 
		   newtoilet->tail->nxt = oldtoilet->tail;
		   oldtoilet->tail->nxt =  newtoilet->tail;//old尾巴的上一個(rev) 
		   newtoilet->tail = oldtoilet->head;
		   oldtoilet->head->pre = NULL;//洗掉記憶前面廁所部分 
		   //revtoilet->head->tag = -1; 改成廁所能否連到rev 
		   //revtoilet->tail->tag = -1;
		 } 
		 /*else//沒必要行動 
		 {
		 	
		 }*/ 
		 
		 
	 }
	 
  	
  }
  else//前面還有可以用的 
  {
  	  revtoilet->head = malloc(sizeof(struct usb));
	  revtoilet->tail= malloc(sizeof(struct usb));
      oldtoilet->tail->Rev = revtoilet;//先malloc才能放在等號後面 
	  oldtoilet->head->Rev = revtoilet;//tail 
	  revtoilet->head = oldtoilet->tail;//只要rev知道誰是頭是尾就好 
	  revtoilet->tail = oldtoilet->head;
	  	   
	  newtoilet->tail->nxt = malloc(sizeof(struct usb));//先malloc
	  newtoilet->tail->nxt = oldtoilet->tail;
	  oldtoilet->tail->nxt = newtoilet->tail;//old尾巴的上一個(rev) 
	  newtoilet->tail = oldtoilet->head;
	  oldtoilet->head->pre = NULL;//洗掉記憶前面廁所部分
   
   
   } 
}
 
void printout(Toilet* toilet,int m)//把結果print
{
	Toilet* newtoilet = toilet;
	struct usb* person;
	for(int i=0;i<m;i++)
	{
	  if(toilet->status == -1)//如果是close
	  {//什麼也不做或continue 
	    toilet++;//還是得前進 
	   } 
	  else
	  {
	  	person = toilet->head;
	  	while(person !=NULL)//感覺會有問題(有malloc的算null?) ,可看也無其他判定法
		 {
		  //補rev部分 
		  printf("%d ",person->id);
		  if(person->Rev==NULL)person = person->nxt;
		  else
		  {
		  	person = printRev(person);//打印完後return,因為還函式內對地址操作不會改變這裡的地址,只有值(tag)會變,除非用指標指標 
		  	
		  	
		   } 
		  //person = nxtorpre(person); //如果tag->-1下一個是用pre 
		 }
		 toilet++;
		 printf("\n"); 
	  }
	  
		
	}
 }

struct usb* printRev(struct usb* person)
{
	//printf("%d ",person->nxt->id);//先把rev頭給印出,已經印過 
	//exit(0);//try 
	if(person->pre!=NULL)
	{ person = person->pre; //下一個 ,if for afraid RE
	
	while(person!=NULL&&person->Rev==NULL)//把中間部分印出,得先保證非NULL,不然指向會錯誤(應該是不會有此錯才對) 
	{
		printf("%d ",person->id);
		person = person->pre; //下一個 
		
	}
	printf("%d ",person->id);//先把rev尾給印出
	if(person->pre !=NULL)person = person->pre; //下一個 ,跳出這個rev link,afraid RE
	else return NULL;
	
	return person; 
	    
	}
	
   else return NULL;//already end
	
}
  

struct usb* nxtorpre(struct usb* person)
{
  if(person->Rev ==NULL)//連不到rev的node,就是正常的 
  {
    return person->nxt;
  }
  else
  {
  	return person->pre;
  	
   } 


}
void assignNxtorpre(struct usb* person)//下面一位 
{
   if(person->Rev ==NULL)
   {
   	person = person->nxt;
   }
   else
   {
   	person = person->pre;
   }

} 





