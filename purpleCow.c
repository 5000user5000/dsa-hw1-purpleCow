#include<stdio.h>
#include<stdlib.h>
#include <string.h> // strtok


//�H 
typedef struct usb
{
    struct usb *pre;
    struct usb *nxt;
    int group;
    int id;
    int tag;//�̦n�Ψ�L��node�ӰO��rev
	struct Toilet *Rev;//�~�M�i�H 
};

//�Z��
typedef struct 
{
    struct usb *head;
    struct usb *tail;
    struct Toilet *nxtRev;//�ΨӰO�U�@��reverse 
    int status;//close or not,�i��bool���N 
}Toilet;//�W�r�\�᭱,����I�s���ι�order���˨C�����n�[struct,�]�i�W���ŧi�@�W,�U���S�ŧi���P�W(���᳣�ϥΦ�) 

//���O 
typedef struct Order
{
	char type;//�x�s���O���Ĥ@�r���ӧP�_enter close leave go
	int num[4];//�̦h�|��3�ӫ��O,�h�Τ@�ӫO�I
	struct Order *nxt;//��U�@�� 
	//struct Order *top;//�̤W��,��K�� ,���G�S�� 
}; 

void leave(Toilet* toilet,int m);
void go(Toilet* toilet,int m);
void enter(Toilet* toilet,int id,int group,int m,struct usb* person);
void close(Toilet* toilet,Toilet* revtoilet,int m,int M);
void printout(Toilet* toilet,int m);
void command(char type,Toilet* toilet,struct Order* order,Toilet* revtoilet,int M); 
struct usb* nxtorpre(struct usb* person);//�T�{�O�_������,���U�@�Ӫ��a�} 
void assignNxtorpre(struct usb* person);//��ȵ��U�@��,i.e.���ʨ�U�@��
struct usb* printRev(struct usb* person); //�L�Xrev�æ^�Ƿ�e�a�} 



int main()
{
  int m,n,k;//�Z�Ҽ� ���O�� ����� 
  char str[40];//�C����O 40���� 
  const char* d = "  ";
  char *p;
  struct Order* order;
  //struct Order* orderTop;
  Toilet *toilet,*revtoilet;//revtoilet�ӰO���ϦV���H 
  int w;
  
  
   order = malloc(sizeof(struct Order));//���ŧi�@�ӴN�n,����A�ʺA�W�[,�o�ˤ���N��ʺAfree�� 
   revtoilet = malloc(sizeof(Toilet));//��l��.�o�˨Smalloc��pre nxt���|���Vnull 
   order = calloc(1,sizeof(struct Order));
   revtoilet = calloc(1,sizeof(Toilet));
   
   struct Order *top = malloc(sizeof(struct Order)); ;//�s�}�Yorder����m 
   top = calloc(1,sizeof(struct Order));//�n�ϥδNcalloc,���M�|�X�� 
   top = order;
 
  //��J 
  scanf("%d",&m);//�ѰO�[& 
  scanf("%d",&n);
  scanf("%d",&k);
  

  
  toilet= malloc(sizeof(Toilet)*m);//�ʺA�W��
  toilet= calloc(m,sizeof(Toilet));
  fgets(str,40,stdin);//����@�Ӧb�e��,�]���Ĥ@�ӥu��\n 
  
  for(int i=0;i<n;i++)//�@�ӭӿ�J���O
  {
    
	fgets(str,40,stdin);//�����scanf(�]���Ů�N��Ū��,�u��fget) 
	w=0;
	//memset(str, 0, 40);//�Ϋe���M�� 
	
    
	p = strtok(str, d); //���J���} ���O�M�Ʀr�H�Ů�j�} 
    order->type = p[0];//get���O���� 
    p = strtok(NULL, d);
    
	while (p != NULL) { //��Ʀr����J��order�W 
       // printf("�o����p = %s\n", p);
        order->num[w++]=atoi(p);//�s�Ʀr,�Τ��ت�string->int ,���Q�Φۤv��,���O�̫�@�Ӽ��`�|�ñ� 
        p = strtok(NULL, d);		   
    }
    if(i != n-1)//�̫�@���N����,�o�˫᭱�~�|���Vnull 
    {
    	order->nxt = malloc(sizeof(struct Order));//���U�@�ӪŶ�
        order->nxt = calloc(1,sizeof(struct Order));
	    order = order->nxt;
    	
	}
    
	
    
  } 
  
  
  while(top!=NULL)
  {
  	//printf("command++ \n");
  	command(top->type,toilet,top,revtoilet,m);//�}�l������O 
  	
  	top = top->nxt;
  }
 
 //printf("print");
 printout(toilet,m);//�L�X���G 
 
 
 /*while(top!=NULL) //��Xlinked list�ݬ� 
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
  
  exit(0);//���� 

  return 0; 
}

void command(char type,Toilet* toilet,struct Order* order,Toilet* revtoilet,int M) 
{
	int com1,com2,com3;//���O1~3 
	switch(type)
	{
	
		case 'e':
			com1 = order->num[0];
			com2 = order->num[1];
			com3 = order->num[2];
			struct usb* person = malloc(sizeof(struct usb));//�o�̫ť�����ζ�? nxt pre�Smalloc 
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

void leave(Toilet* toilet,int m)//�����tag rev.. 
{
	Toilet* newtoilet = toilet +m;//���V��m�Z��address
	if(newtoilet->tail->Rev== NULL)
	{
	  newtoilet->tail =newtoilet->tail->pre;//��W�@�ӳs��Z�Ҫ�����
	  newtoilet->tail->nxt = NULL;//����ڪ��U�@�ӥh�� 
	
	} 
    else {
    	Toilet* revtoilet = newtoilet->tail->Rev;//����rev���_�� 
		newtoilet->tail =newtoilet->tail->nxt;//�˹L��
		//newtoilet = newtoilet->tail;newtoilet->tail���ӬOusb�� �Dtoilet�� 
		
		//�ǩ�rev 
		if(revtoilet->head == revtoilet->tail)
		{
		  //do nothing,�̫�@��N���|�A�ǩӤF  
		}
		else
		{
		 revtoilet->tail = newtoilet->tail;
		 newtoilet->tail->Rev =  revtoilet;//���褬�۬���,here modify(toilet should not use usb assign)
		
		} 
		//printf("leave id = %d \n",newtoilet->tail->id);//to check
		//printf("leave pre id = %d \n",newtoilet->tail->pre->id);//to check
    	newtoilet->tail->pre = NULL;//����ڪ��U�@�ӥh�� 
	}

	//�nfree? ���Ӥ��� ,�ϰ��ܼ� 
	 
}
void go(Toilet* toilet,int m)
{
	Toilet* newtoilet = toilet +m;//���V��m�Z��address
	//newtoilet = newtoilet->head; �H�M�Z�Ҥ��P�� 
	//assignNxtorpre(newtoilet->head);//���N�U�������O 
	//newtoilet->head = newtoilet->head->nxt;//......�p�G�S��nxt�|���~��?
	if(newtoilet->head->Rev !=NULL)//�p�G��}�Y�Orev 
	{
		Toilet* revtoilet = newtoilet->head->Rev;
		if(revtoilet->head == revtoilet->tail){//�p�Grev��head�Mtail���w�ۦP->�̫�@��rev 
			//do nothing,�̫�@��N���|�A�ǩӤF 
		}
		else
		{
		  revtoilet->head = newtoilet->head->pre;//��rev�ǵ��U���@��,���� newtoilet->head->Rev->head...�b�Y�����Ӧh
		  newtoilet->head->pre->Rev = revtoilet;//���۬���,�o��O��rev�ǤU�h 
		} 
		//�ǩӧ���,�����Y 
		newtoilet->head =  newtoilet->head->pre;//
		newtoilet->head->nxt = newtoilet;//���V nxt not pre
	} 
	else
	{
	  newtoilet->head =  newtoilet->head->nxt;
	  if(newtoilet->head->Rev !=NULL)newtoilet->head->nxt = newtoilet;//�Y���W�@�ӬO���V�Z�� 
	  else newtoilet->head->pre = newtoilet;//�p�G�U�@�ӬOrev	  
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
		newtoilet->head = calloc(1,sizeof(struct Order));//���ӥi�� 
		newtoilet->head = person;
		newtoilet->tail = malloc(sizeof(struct Order));//�ѤF�ɤW 
		newtoilet->tail =  person;
		return ;//�[�ֵ��� 
		
	}
	
	else
	{
	  struct usb* others= malloc(sizeof(struct usb));
	  //others= calloc(1,sizeof(struct usb));
	  others= newtoilet->head ;//�ΨӲ���
	  
	  while(nxtorpre(others)!=NULL&&others->group != person->group )//�]�i���others id���ӧP�w ,�٭n��rev //->nxt
	  {
	  	if(others->Rev==NULL){
	  		person->pre = others;
	  	    others = others->nxt;
		  }
		else
		{
			person->nxt = others;//�e�� 
	  	    others = others->pre;
		}
	  	
	  }
	  //while����2caces,1�O��null(�N����m���w��person) 
	  if(nxtorpre(others)==NULL)//�S�Ҽ{��5(0) 8(1) 7(0) �o�ئb8���ɭ�,�ŦXtag�Mnext�����D,���O�o�̿��~ //->nxt
	  {
	  	person->pre = others;//
	  	//others->nxt = malloc(sizeof(struct usb));
	  	//others->nxt = calloc(1,sizeof(struct usb));
	  	if(others->Rev==NULL)others->nxt = person;//null = person,�o�̪�null�Dother->nxt 
	  	else others->pre = person; 
	  	newtoilet->tail = person;//tail�O�ѸɤW 
	  }
	  else//2�O�p�G�J��Pid�� 
	  {
	  	while(nxtorpre(others) !=NULL&&others->group == person->group)//����令�Pid ,null�n���\�n�e��,�]��null�S�k���Vgroup 
	  	{
	  	  //person->pre = malloc(sizeof(struct usb)); 
	  	  //person->pre = calloc(1,sizeof(struct usb));
		  if(others->Rev==NULL){
	  		person->pre = others;
	  	    others = others->nxt;
		  }
	     else
		  {
			person->pre = others;//�e��,person�O���`��,���Χ� 
	  	    others = others->pre;
		   }
		} 
	    
	    if(others->group != person->group)//�Ҧb��e�� 
	    {
		   //others->pre = person->pre;
		   
		   if(others->Rev==NULL &&others->pre->Rev==NULL)others->pre->nxt = person;//�@�뫬 && others->pre!=NULL
		   else if(others->Rev!=NULL &&others->pre->Rev!=NULL)others->nxt->pre = person;
		   else if(others->Rev==NULL &&others->pre->Rev!=NULL)others->pre->pre = person;
		   else if(others->Rev!=NULL &&others->pre->Rev==NULL)others->nxt->nxt = person;
		   //person->nxt =  malloc(sizeof(struct usb));
		   //person->pre =  malloc(sizeof(struct usb));
		   person->nxt = others ;//�����null�����O�H
		   if(others->Rev==NULL)others->pre = person ; 
		   else others->nxt = person ; 
		   
	    	
		}
		else // others->nxt ==NULL
	    {
	    	person->pre = others;
	     	if(others->Rev==NULL)others->nxt = person;
	     	else others->pre = person;
	    	newtoilet->tail = person;//tail�O�ѸɤW 
	    }
	  }
	}
	
 } 
void close(Toilet* toilet,Toilet* revtoilet,int m,int M)//m�O���w����m,M�O�Z���`�� 
{
  Toilet* oldtoilet = toilet+m;//�ª��Z�� 
  Toilet* newtoilet = toilet+m;//�s�� 
  newtoilet->status = -1;//�������
  while(newtoilet != NULL&& newtoilet->status == -1) //��S�J��Ū��B�D�W�L�Ĥ@�ӴZ��(�i�঳���D) 
  {
     newtoilet--; //���e�� 
  } 
  //2�i�� 1�O�e�������F 2�O��즳
  if(newtoilet == NULL)//�e������
  {
     newtoilet = toilet;//���s���^
     if(m=M)//�w�g�O�̫�@��
	 {
	   free(newtoilet->head);//��o��free 
	   free(newtoilet->tail);//��o��free 
	 } 
	 else
	 {
	 	newtoilet++;
		 while(newtoilet != NULL && newtoilet->status ==-1)//��O��newtoilet == toilet+M 
		 {newtoilet++;}//����� 
		 if(newtoilet != NULL)//�٦���m���� ,�B�Oopen 
		 {
		   //revtoilet->head = oldtoilet->tail;
		   //revtoilet->tail = oldtoilet->head;
		   //������ 
		   revtoilet->head = malloc(sizeof(struct usb));
	       revtoilet->tail= malloc(sizeof(struct usb));
		   oldtoilet->tail->Rev = revtoilet;// ->head �H�u�n����rev�N�n,tail head�᭱�A���t 
		   oldtoilet->head->Rev = revtoilet;// ->tail
		   revtoilet->head = oldtoilet->tail;//�u�nrev���D�֬O�Y�O���N�n 
		   revtoilet->tail = oldtoilet->head;
		   
		   newtoilet->tail->nxt = malloc(sizeof(struct usb));//��malloc,�i��i�� 
		   newtoilet->tail->nxt = oldtoilet->tail;
		   oldtoilet->tail->nxt =  newtoilet->tail;//old���ڪ��W�@��(rev) 
		   newtoilet->tail = oldtoilet->head;
		   oldtoilet->head->pre = NULL;//�~���O�Ыe���Z�ҳ��� 
		   //revtoilet->head->tag = -1; �令�Z�ү�_�s��rev 
		   //revtoilet->tail->tag = -1;
		 } 
		 /*else//�S���n��� 
		 {
		 	
		 }*/ 
		 
		 
	 }
	 
  	
  }
  else//�e���٦��i�H�Ϊ� 
  {
  	  revtoilet->head = malloc(sizeof(struct usb));
	  revtoilet->tail= malloc(sizeof(struct usb));
      oldtoilet->tail->Rev = revtoilet;//��malloc�~���b�����᭱ 
	  oldtoilet->head->Rev = revtoilet;//tail 
	  revtoilet->head = oldtoilet->tail;//�u�nrev���D�֬O�Y�O���N�n 
	  revtoilet->tail = oldtoilet->head;
	  	   
	  newtoilet->tail->nxt = malloc(sizeof(struct usb));//��malloc
	  newtoilet->tail->nxt = oldtoilet->tail;
	  oldtoilet->tail->nxt = newtoilet->tail;//old���ڪ��W�@��(rev) 
	  newtoilet->tail = oldtoilet->head;
	  oldtoilet->head->pre = NULL;//�~���O�Ыe���Z�ҳ���
   
   
   } 
}
 
void printout(Toilet* toilet,int m)//�⵲�Gprint
{
	Toilet* newtoilet = toilet;
	struct usb* person;
	for(int i=0;i<m;i++)
	{
	  if(toilet->status == -1)//�p�G�Oclose
	  {//����]������continue 
	    toilet++;//�٬O�o�e�i 
	   } 
	  else
	  {
	  	person = toilet->head;
	  	while(person !=NULL)//�Pı�|�����D(��malloc����null?) ,�i�ݤ]�L��L�P�w�k
		 {
		  //��rev���� 
		  printf("%d ",person->id);
		  if(person->Rev==NULL)person = person->nxt;
		  else
		  {
		  	person = printRev(person);//���L����return,�]���٨禡����a�}�ާ@���|���ܳo�̪��a�},�u����(tag)�|��,���D�Ϋ��Ы��� 
		  	
		  	
		   } 
		  //person = nxtorpre(person); //�p�Gtag->-1�U�@�ӬO��pre 
		 }
		 toilet++;
		 printf("\n"); 
	  }
	  
		
	}
 }

struct usb* printRev(struct usb* person)
{
	//printf("%d ",person->nxt->id);//����rev�Y���L�X,�w�g�L�L 
	//exit(0);//try 
	if(person->pre!=NULL)
	{ person = person->pre; //�U�@�� ,if for afraid RE
	
	while(person!=NULL&&person->Rev==NULL)//�⤤�������L�X,�o���O�ҫDNULL,���M���V�|���~(���ӬO���|�������~��) 
	{
		printf("%d ",person->id);
		person = person->pre; //�U�@�� 
		
	}
	printf("%d ",person->id);//����rev�����L�X
	if(person->pre !=NULL)person = person->pre; //�U�@�� ,���X�o��rev link,afraid RE
	else return NULL;
	
	return person; 
	    
	}
	
   else return NULL;//already end
	
}
  

struct usb* nxtorpre(struct usb* person)
{
  if(person->Rev ==NULL)//�s����rev��node,�N�O���`�� 
  {
    return person->nxt;
  }
  else
  {
  	return person->pre;
  	
   } 


}
void assignNxtorpre(struct usb* person)//�U���@�� 
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





