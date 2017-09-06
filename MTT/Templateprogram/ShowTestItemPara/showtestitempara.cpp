#include "showtestitempara.h"
#include "ui_showtestitempara.h"
#include <QTextCodec>

ShowTestitemPara::ShowTestitemPara(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowTestitemPara)
{
    ui->setupUi(this);
    setWindowTitle(QString(" "));
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QFont font("wenquanyi",16);
    setFont(font);
    ui->pushButton->setFocusPolicy(Qt::NoFocus);


}

ShowTestitemPara::~ShowTestitemPara()
{
    delete ui;
}

void  ShowTestitemPara::Setshowpara(struct SECTMENTSET value,int type,int item)
{

    memcpy(&Sectvalue,&value,sizeof(struct SECTMENTSET));
    Protecttype=type; //保护类型
    CurrentItem=item; //当前测试项目

    switch(Protecttype)
    {
    case 0: //电压
         ShowVoltagePara(item);
          break;

    case 1: //电流
         ShowCurrentPara(item);

          break;

    case 2: //方向
         ShowPowerPara();

          break;

    case 3: //频率
         ShowFreqPara(item);
          break;


    case 4: //任意测试
          break;

    case 5: //相位
        ShowPhasePara(item);
         break;

    case 6: //谐波
        ShowHarmonicPara();
         break;
    }








}

void  ShowTestitemPara::SetSettingLink(struct SETTINGNODELIST *link)
{
  settinglink=link;
}

void ShowTestitemPara::ShowVoltagePara(int type)
{

  if(type==0) //pick up
   {
    ui->textBrowser->append(tr("Name:")+tr("Pick up"));
    CaculateVariable(settinglink,Sectvalue.volset.FaultType); //故障故障类型
    int faulttype=(int)Sectvalue.volset.FaultType.value;
    switch(faulttype)
    {
    case 1:
         ui->textBrowser->append(tr("Fault Type:")+tr("A-E"));
        break;
    case 2:
        ui->textBrowser->append(tr("Fault Type:")+tr("B-E"));
        break;
    case 3:
        ui->textBrowser->append(tr("Fault Type:")+tr("C-E"));
        break;

    case 4:
        ui->textBrowser->append(tr("Fault Type:")+tr("A-B"));
        break;
    case 5:
         ui->textBrowser->append(tr("Fault Type:")+tr("B-C"));
        break;
    case 6:
         ui->textBrowser->append(tr("Fault Type:")+tr("C-A"));

        break;
    case 7:
        ui->textBrowser->append(tr("Fault Type:")+tr("A-B-C"));
        break;
    }


    ui->textBrowser->append(tr("Tripping value keeping(s):")+QString(Sectvalue.volset.Faultduraion.text));
    ui->textBrowser->append(tr("Start Voltage(V):")+QString(Sectvalue.volset.TripValue.StartValue.text));
    ui->textBrowser->append(tr("End Voltage(V):")+QString(Sectvalue.volset.TripValue.EndValue.text));
    ui->textBrowser->append(tr("Step Voltage(V):")+QString(Sectvalue.volset.TripValue.StepValue.text));
    ui->textBrowser->append(tr("Step Time(s):")+QString(Sectvalue.volset.TripValue.HoldTime.text));


    if(Sectvalue.volset.TripValue.assesstripvalue.relerror.valid==true\
            &&Sectvalue.volset.TripValue.assesstripvalue.abserror.valid!=true)
    {
        char tmpstr1[512];
        sprintf(tmpstr1,"Criterion=REL(%f)",Sectvalue.volset.TripValue.assesstripvalue.relerror.errorvalue);
        ui->textBrowser->append(tr(tmpstr1));
    }

    else if(Sectvalue.volset.TripValue.assesstripvalue.abserror.valid==true\
            &&Sectvalue.volset.TripValue.assesstripvalue.relerror.valid!=true)
    {
        char tmpstr1[512];
        sprintf(tmpstr1,"Criterion=ABS(%f,%f)",\
        Sectvalue.volset.TripValue.assesstripvalue.abserror.errorvalue,\
        Sectvalue.volset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);
        ui->textBrowser->append(tr(tmpstr1));

    }


    else  if(Sectvalue.volset.TripValue.assesstripvalue.relerror.valid==true&&\
    Sectvalue.volset.TripValue.assesstripvalue.abserror.valid==true)
    {

        if(Sectvalue.volset.TripValue.assesstripvalue.assessandor==1) //与的关系
        {
             char tmpstr1[512];
             sprintf(tmpstr1,"Criterion=REL(%f)&ABS(%f,%f)",\
             Sectvalue.volset.TripValue.assesstripvalue.relerror.errorvalue,\
             Sectvalue.volset.TripValue.assesstripvalue.abserror.errorvalue,\
             Sectvalue.volset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);
             ui->textBrowser->append(tr(tmpstr1));

        }
        else if(Sectvalue.volset.TripValue.assesstripvalue.assessandor==2)
        {
            char tmpstr1[512];
             sprintf(tmpstr1,"Criterion=REL(%f)|ABS(%f,%f)",\
             Sectvalue.volset.TripValue.assesstripvalue.relerror.errorvalue,\
             Sectvalue.volset.TripValue.assesstripvalue.abserror.errorvalue,\
             Sectvalue.volset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);
             ui->textBrowser->append(tr(tmpstr1));
        }
    }




   }
  else if(type==1) //
  {
      if(Sectvalue.volset.TripTime.assesstriptime.abserror.errorvalue==-1&&\
         Sectvalue.volset.TripTime.assesstriptime.abserror.errorvalue_Negtive==-1)
      ui->textBrowser->append(tr("Name:")+tr("Voltage Stability"));
      else
      ui->textBrowser->append(tr("Name:")+tr("Trip time"));
      CaculateVariable(settinglink,Sectvalue.volset.FaultType); //故障故障类型
      int faulttype=(int)Sectvalue.volset.FaultType.value;
      switch(faulttype)
      {
      case 1:
           ui->textBrowser->append(tr("Fault Type:")+tr("A-E"));
          break;
      case 2:
          ui->textBrowser->append(tr("Fault Type:")+tr("B-E"));
          break;
      case 3:
          ui->textBrowser->append(tr("Fault Type:")+tr("C-E"));
          break;

      case 4:
          ui->textBrowser->append(tr("Fault Type:")+tr("A-B"));
          break;
      case 5:
           ui->textBrowser->append(tr("Fault Type:")+tr("B-C"));
          break;
      case 6:
           ui->textBrowser->append(tr("Fault Type:")+tr("C-A"));

          break;
      case 7:
          ui->textBrowser->append(tr("Fault Type:")+tr("A-B-C"));
          break;
      }

      if(Sectvalue.volset.TripTime.UseMeasureValue==false)
      ui->textBrowser->append(tr("Voltage End(V):")+QString(Sectvalue.volset.TripTime.EndVaule.text));
      else
      ui->textBrowser->append(tr("Voltage End(V):Test Result"));
      ui->textBrowser->append(tr("Hold Time(s):")+QString(Sectvalue.volset.TripTime.HoldTime.text));
      bool Showassess=(Sectvalue.volset.TripTime.assesstriptime.abserror.errorvalue==-1&&\
                         Sectvalue.volset.TripTime.assesstriptime.abserror.errorvalue_Negtive==-1);
      if(Showassess==false)
      {

          if(Sectvalue.volset.TripTime.assesstriptime.relerror.valid==true&&Sectvalue.volset.TripTime.assesstriptime.abserror.valid!=true)
          {
              char tmpstr1[512];
              sprintf(tmpstr1,"Criterion=REL(%f)",Sectvalue.volset.TripTime.assesstriptime.relerror.errorvalue);
              ui->textBrowser->append(tr(tmpstr1));
          }

         else if(Sectvalue.volset.TripTime.assesstriptime.abserror.valid==true&&Sectvalue.volset.TripTime.assesstriptime.relerror.valid!=true)
          {
              char tmpstr1[512];
              sprintf(tmpstr1,"Criterion=ABS(%f,%f)",\
              Sectvalue.volset.TripTime.assesstriptime.abserror.errorvalue,\
              Sectvalue.volset.TripTime.assesstriptime.abserror.errorvalue_Negtive);

              ui->textBrowser->append(tr(tmpstr1));

          }
         else  if(Sectvalue.volset.TripTime.assesstriptime.relerror.valid==true&&\
             Sectvalue.volset.TripTime.assesstriptime.abserror.valid==true)
          {

              if(Sectvalue.volset.TripTime.assesstriptime.assessandor==1) //与的关系
              {
                   char tmpstr1[512];
                   sprintf(tmpstr1,"Criterion=REL(%f)&ABS(%f,%f)",\
                   Sectvalue.volset.TripTime.assesstriptime.relerror.errorvalue,\
                   Sectvalue.volset.TripTime.assesstriptime.abserror.errorvalue,\
                   Sectvalue.volset.TripTime.assesstriptime.abserror.errorvalue_Negtive);
                   ui->textBrowser->append(tr(tmpstr1));


              }
              else if(Sectvalue.volset.TripTime.assesstriptime.assessandor==2)
              {
                   char tmpstr1[512];
                  sprintf(tmpstr1,"Criterion=REL(%f)|ABS(%f,%f)",\
                  Sectvalue.volset.TripTime.assesstriptime.relerror.errorvalue,\
                  Sectvalue.volset.TripTime.assesstriptime.abserror.errorvalue,\
                  Sectvalue.volset.TripTime.assesstriptime.abserror.errorvalue_Negtive);
                  ui->textBrowser->append(tr(tmpstr1));

              }
          }



      }


  }


}

void ShowTestitemPara::ShowCurrentPara(int type)
{

    if(type==0) //pick up
         {
          ui->textBrowser->append(tr("Name:")+tr("Pick up"));
          CaculateVariable(settinglink,Sectvalue.curset.output); //故障故障类型
          int faulttype=(int)Sectvalue.curset.output.value;
          switch(faulttype)
          {
          case 1:
               ui->textBrowser->append(tr("Output Selection:")+tr("IA"));
              break;
          case 2:
              ui->textBrowser->append(tr("Output Selection:")+tr("IB"));
              break;
          case 3:
              ui->textBrowser->append(tr("Output Selection:")+tr("IC"));
              break;

          case 4:
              ui->textBrowser->append(tr("Output Selection:")+tr("IABC"));
              break;
          }


          ui->textBrowser->append(tr("Tripping value keeping(s):")+QString(Sectvalue.curset.Faultduraion.text));
          ui->textBrowser->append(tr("Start Current(A):")+QString(Sectvalue.curset.TripValue.StartValue.text));
          ui->textBrowser->append(tr("End Current(A):")+QString(Sectvalue.curset.TripValue.EndValue.text));
          ui->textBrowser->append(tr("Step Current(A):")+QString(Sectvalue.curset.TripValue.StepValue.text));
          ui->textBrowser->append(tr("Step Time(s):")+QString(Sectvalue.curset.TripValue.HoldTime.text));


          if(Sectvalue.curset.TripValue.assesstripvalue.relerror.valid==true\
                  &&Sectvalue.curset.TripValue.assesstripvalue.abserror.valid!=true)
          {
              char tmpstr1[512];
              sprintf(tmpstr1,"Criterion=REL(%f)",Sectvalue.curset.TripValue.assesstripvalue.relerror.errorvalue);
              ui->textBrowser->append(tr(tmpstr1));
          }

          else if(Sectvalue.curset.TripValue.assesstripvalue.abserror.valid==true\
                  &&Sectvalue.curset.TripValue.assesstripvalue.relerror.valid!=true)
          {
              char tmpstr1[512];
              sprintf(tmpstr1,"Criterion=ABS(%f,%f)",\
              Sectvalue.curset.TripValue.assesstripvalue.abserror.errorvalue,\
              Sectvalue.curset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);
              ui->textBrowser->append(tr(tmpstr1));

          }


          else  if(Sectvalue.curset.TripValue.assesstripvalue.relerror.valid==true&&\
          Sectvalue.curset.TripValue.assesstripvalue.abserror.valid==true)
          {

              if(Sectvalue.curset.TripValue.assesstripvalue.assessandor==1) //与的关系
              {
                   char tmpstr1[512];
                   sprintf(tmpstr1,"Criterion=REL(%f)&ABS(%f,%f)",\
                   Sectvalue.curset.TripValue.assesstripvalue.relerror.errorvalue,\
                   Sectvalue.curset.TripValue.assesstripvalue.abserror.errorvalue,\
                   Sectvalue.curset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);
                   ui->textBrowser->append(tr(tmpstr1));

              }
              else if(Sectvalue.curset.TripValue.assesstripvalue.assessandor==2)
              {
                  char tmpstr1[512];
                   sprintf(tmpstr1,"Criterion=REL(%f)|ABS(%f,%f)",\
                   Sectvalue.curset.TripValue.assesstripvalue.relerror.errorvalue,\
                   Sectvalue.curset.TripValue.assesstripvalue.abserror.errorvalue,\
                   Sectvalue.curset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);
                   ui->textBrowser->append(tr(tmpstr1));
              }
          }




         }

    else if(type==1) //
        {
            if(Sectvalue.curset.TripTime.assesstriptime.abserror.errorvalue==-1&&\
               Sectvalue.curset.TripTime.assesstriptime.abserror.errorvalue_Negtive==-1)
            ui->textBrowser->append(tr("Name:")+tr("Current Stability"));
            else
            ui->textBrowser->append(tr("Name:")+tr("Trip time"));

            CaculateVariable(settinglink,Sectvalue.curset.output); //故障故障类型
            int faulttype=(int)Sectvalue.curset.output.value;
            switch(faulttype)
            {
            case 1:
                 ui->textBrowser->append(tr("Output Selection:")+tr("IA"));
                break;
            case 2:
                ui->textBrowser->append(tr("Output Selection:")+tr("IB"));
                break;
            case 3:
                ui->textBrowser->append(tr("Output Selection:")+tr("IC"));
                break;

            case 4:
                ui->textBrowser->append(tr("Output Selection:")+tr("IABC"));
                break;
            }


            ui->textBrowser->append(tr("Current End(A):")+QString(Sectvalue.curset.TripTime.EndVaule.text));
            ui->textBrowser->append(tr("Hold Time(s):")+QString(Sectvalue.curset.TripTime.HoldTime.text));
            bool Showassess=(Sectvalue.curset.TripTime.assesstriptime.abserror.errorvalue==-1&&\
                               Sectvalue.curset.TripTime.assesstriptime.abserror.errorvalue_Negtive==-1);
            if(Showassess==false)
            {

                if(Sectvalue.curset.TripTime.assesstriptime.relerror.valid==true&&Sectvalue.curset.TripTime.assesstriptime.abserror.valid!=true)
                {
                    char tmpstr1[512];
                    sprintf(tmpstr1,"Criterion=REL(%f)",Sectvalue.curset.TripTime.assesstriptime.relerror.errorvalue);
                    ui->textBrowser->append(tr(tmpstr1));
                }

               else if(Sectvalue.curset.TripTime.assesstriptime.abserror.valid==true&&Sectvalue.curset.TripTime.assesstriptime.relerror.valid!=true)
                {
                    char tmpstr1[512];
                    sprintf(tmpstr1,"Criterion=ABS(%f,%f)",\
                    Sectvalue.curset.TripTime.assesstriptime.abserror.errorvalue,\
                    Sectvalue.curset.TripTime.assesstriptime.abserror.errorvalue_Negtive);

                    ui->textBrowser->append(tr(tmpstr1));

                }
               else  if(Sectvalue.curset.TripTime.assesstriptime.relerror.valid==true&&\
                   Sectvalue.curset.TripTime.assesstriptime.abserror.valid==true)
                {

                    if(Sectvalue.curset.TripTime.assesstriptime.assessandor==1) //与的关系
                    {
                         char tmpstr1[512];
                         sprintf(tmpstr1,"Criterion=REL(%f)&ABS(%f,%f)",\
                         Sectvalue.curset.TripTime.assesstriptime.relerror.errorvalue,\
                         Sectvalue.curset.TripTime.assesstriptime.abserror.errorvalue,\
                         Sectvalue.curset.TripTime.assesstriptime.abserror.errorvalue_Negtive);
                         ui->textBrowser->append(tr(tmpstr1));


                    }
                    else if(Sectvalue.curset.TripTime.assesstriptime.assessandor==2)
                    {
                         char tmpstr1[512];
                        sprintf(tmpstr1,"Criterion=REL(%f)|ABS(%f,%f)",\
                        Sectvalue.curset.TripTime.assesstriptime.relerror.errorvalue,\
                        Sectvalue.curset.TripTime.assesstriptime.abserror.errorvalue,\
                        Sectvalue.curset.TripTime.assesstriptime.abserror.errorvalue_Negtive);
                        ui->textBrowser->append(tr(tmpstr1));
                    }
                }



            }


        }


}

void ShowTestitemPara::ShowPowerPara()
{
   ui->textBrowser->append(tr("Name:")+tr("Power Direction"));
   ui->textBrowser->append(tr("Fault Current(A):")+tr(Sectvalue.dirset.TripMta.Ifault.text));
   ui->textBrowser->append(tr("Fault Voltage(V):")+tr(Sectvalue.dirset.TripMta.vfault.text));
   ui->textBrowser->append(tr("Mta(Deg):")+tr(Sectvalue.dirset.TripMta.mta.text));

   CaculateVariable(settinglink,Sectvalue.dirset.TripMta.faulttype); //故障故障类型
   int faulttype=atoi(Sectvalue.dirset.TripMta.faulttype.text);
   switch(faulttype)
   {
   case 1:
       ui->textBrowser->append(tr("Fault Type:A-E"));
       break;
   case 2:
       ui->textBrowser->append(tr("Fault Type:B-E"));
       break;
   case 3:
       ui->textBrowser->append(tr("Fault Type:C-E"));
       break;
   case 4:
       ui->textBrowser->append(tr("Fault Type:A-B-C"));
       break;
   }
   ui->textBrowser->append(tr("Step(Deg):")+tr(Sectvalue.dirset.TripMta.step.text));
   ui->textBrowser->append(tr("Steptime(s):")+tr(Sectvalue.dirset.TripMta.steptime.text));


 //  ui->textBrowser->append(tr("Criterion:")+tr(settinglink,Sectvalue.dirset.TripMta.));
   if(Sectvalue.dirset.TripMta.assessmta.relerror.valid==true\
    &&Sectvalue.dirset.TripMta.assessmta.abserror.valid!=true)
   {
       char tmpstr1[512];
       sprintf(tmpstr1,"Criterion=REL(%f)",Sectvalue.dirset.TripMta.assessmta.relerror.errorvalue);

       ui->textBrowser->append(tr(tmpstr1));

   }

   else if(Sectvalue.dirset.TripMta.assessmta.abserror.valid==true&&\
          Sectvalue.dirset.TripMta.assessmta.relerror.valid!=true)
   {
       char tmpstr1[512];
       sprintf(tmpstr1,"Criterion=ABS(%f,%f)",\
       Sectvalue.dirset.TripMta.assessmta.abserror.errorvalue,\
       Sectvalue.dirset.TripMta.assessmta.abserror.errorvalue_Negtive);
        ui->textBrowser->append(tr(tmpstr1));


   }
   else   if(Sectvalue.dirset.TripMta.assessmta.relerror.valid==true&&\
      Sectvalue.dirset.TripMta.assessmta.abserror.valid==true)
   {
       if(Sectvalue.dirset.TripMta.assessmta.assessandor==1) //与的关系
       {
           char strbuf[512];
            sprintf(strbuf,"Criterion=REL(%f)&ABS(%f,%f)",\
            Sectvalue.dirset.TripMta.assessmta.relerror.errorvalue,\
            Sectvalue.dirset.TripMta.assessmta.abserror.errorvalue,\
            Sectvalue.dirset.TripMta.assessmta.abserror.errorvalue_Negtive);
            ui->textBrowser->append(tr(strbuf));

       }
       else if(Sectvalue.dirset.TripMta.assessmta.assessandor==2)
       {
           char strbuf[512];
           sprintf(strbuf,"Criterion=REL(%f)|ABS(%f,%f)",\
           Sectvalue.dirset.TripMta.assessmta.relerror.errorvalue,\
           Sectvalue.dirset.TripMta.assessmta.abserror.errorvalue,\
           Sectvalue.dirset.TripMta.assessmta.abserror.errorvalue_Negtive);
          ui->textBrowser->append(tr(strbuf));
       }
   }
}

void ShowTestitemPara::ShowFreqPara(int type)
{
  if(type==0) //pick up
  {
  ui->textBrowser->append(tr("Name:")+tr("Pick up"));
  ui->textBrowser->append(tr("Tripping value keeping(s):")+QString(Sectvalue.freset.Faultduraion.text));
  ui->textBrowser->append(tr("Start Frequency(Hz):")+QString(Sectvalue.freset.TripValue.FreEndValueStart.text));
  ui->textBrowser->append(tr("End Frequency(Hz):")+QString(Sectvalue.freset.TripValue.FreEndValueEnd.text));
  ui->textBrowser->append(tr("Frequency Step(Hz):")+QString(Sectvalue.freset.TripValue.FreStep.text));
  ui->textBrowser->append(tr("Step Time(s):")+QString(Sectvalue.freset.TripValue.HoldTime.text));

  if(Sectvalue.freset.TripValue.assesstripvalue.relerror.valid==true&&\
     Sectvalue.freset.TripValue.assesstripvalue.abserror.valid!=true)
  {
      char tmpstr1[512];
      sprintf(tmpstr1,"Criterion=REL(%f)",Sectvalue.freset.TripValue.assesstripvalue.relerror.errorvalue);
      ui->textBrowser->append(tr(tmpstr1));

  }



 else if(Sectvalue.freset.TripValue.assesstripvalue.abserror.valid==true&&\
         Sectvalue.freset.TripValue.assesstripvalue.relerror.valid!=true)
  {
      char tmpstr1[512];
      sprintf(tmpstr1,"Criterion=ABS(%f,%f)",\
      Sectvalue.freset.TripValue.assesstripvalue.abserror.errorvalue,\
      Sectvalue.freset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);
      ui->textBrowser->append(tr(tmpstr1));

  }
 else   if(Sectvalue.freset.TripValue.assesstripvalue.relerror.valid==true&&\
           Sectvalue.freset.TripValue.assesstripvalue.abserror.valid==true)
  {
      if(Sectvalue.freset.TripValue.assesstripvalue.assessandor==1) //与的关系
      {
          char tmpstr1[512];
          sprintf(tmpstr1,"Criterion=REL(%f)&ABS(%f,%f)",\
          Sectvalue.freset.TripValue.assesstripvalue.relerror.errorvalue,\
          Sectvalue.freset.TripValue.assesstripvalue.abserror.errorvalue,\
          Sectvalue.freset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);
          ui->textBrowser->append(tr(tmpstr1));

      }
      else if(Sectvalue.freset.TripValue.assesstripvalue.assessandor==2)
      {
          char tmpstr1[512];
          sprintf(tmpstr1,"Criterion=REL(%f)|ABS(%f,%f)",\
          Sectvalue.freset.TripValue.assesstripvalue.relerror.errorvalue,\
          Sectvalue.freset.TripValue.assesstripvalue.abserror.errorvalue,\
          Sectvalue.freset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);
          ui->textBrowser->append(tr(tmpstr1));

      }

  }





  }
  else if(type==1)
  {
      ui->textBrowser->append(tr("Name:")+tr("Trip Time"));
      ui->textBrowser->append(tr("Tripping value keeping(s):")+QString(Sectvalue.freset.Faultduraion.text));
      ui->textBrowser->append(tr("Frequency End(Hz):")+QString(Sectvalue.freset.TripTime.FreEndValue.text));
      ui->textBrowser->append(tr("Hold Time(s):")+QString(Sectvalue.freset.TripTime.HoldTime.text));

      if(Sectvalue.freset.TripTime.assesstriptime.relerror.valid==true\
       &&Sectvalue.freset.TripTime.assesstriptime.abserror.valid!=true)
      {
          char tmpstr1[512];
          sprintf(tmpstr1,"Criterion=REL(%f)",Sectvalue.freset.TripTime.assesstriptime.relerror.errorvalue);
          ui->textBrowser->append(tr(tmpstr1));
      }

     else if(Sectvalue.freset.TripTime.assesstriptime.abserror.valid==true\
             &&Sectvalue.freset.TripTime.assesstriptime.relerror.valid!=true)
      {
          char tmpstr1[512];
          sprintf(tmpstr1,"Criterion=ABS(%f,%f)",\
          Sectvalue.freset.TripTime.assesstriptime.abserror.errorvalue,\
          Sectvalue.freset.TripTime.assesstriptime.abserror.errorvalue_Negtive);
          ui->textBrowser->append(tr(tmpstr1));


      }
     else   if(Sectvalue.freset.TripTime.assesstriptime.relerror.valid==true&&\
               Sectvalue.freset.TripTime.assesstriptime.abserror.valid==true)
      {

          if(Sectvalue.freset.TripTime.assesstriptime.assessandor==1) //与的关系
          {
              char tmpstr1[512];
              sprintf(tmpstr1,"Criterion=REL(%f)&ABS(%f,%f)",\
              Sectvalue.freset.TripTime.assesstriptime.relerror.errorvalue,\
              Sectvalue.freset.TripTime.assesstriptime.abserror.errorvalue,\
              Sectvalue.freset.TripTime.assesstriptime.abserror.errorvalue_Negtive);
              ui->textBrowser->append(tr(tmpstr1));

          }
          else if(Sectvalue.freset.TripTime.assesstriptime.assessandor==2)
          {
              char tmpstr1[512];
              sprintf(tmpstr1,"Criterion=REL(%f)|ABS(%f,%f)",\
              Sectvalue.freset.TripTime.assesstriptime.relerror.errorvalue,\
              Sectvalue.freset.TripTime.assesstriptime.abserror.errorvalue,\
              Sectvalue.freset.TripTime.assesstriptime.abserror.errorvalue_Negtive);
              ui->textBrowser->append(tr(tmpstr1));
          }
      }



  }

  else if(type==2)
  {
      ui->textBrowser->append(tr("Name:")+tr("df/dt blocking"));
      ui->textBrowser->append(tr("Tripping value keeping(s):")+QString(Sectvalue.freset.Faultduraion.text));
      ui->textBrowser->append(tr("Frequency End(Hz):")+QString(Sectvalue.freset.Tripdfdt.FreEndValue.text));
      ui->textBrowser->append(tr("df/dt Start(Hz/s):")+QString(Sectvalue.freset.Tripdfdt.DfdtStart.text));
      ui->textBrowser->append(tr("df/dt End(Hz/s):")+QString(Sectvalue.freset.Tripdfdt.DfdtEnd.text));
      ui->textBrowser->append(tr("df/dt Step(Hz/s):")+QString(Sectvalue.freset.Tripdfdt.DfdtStep.text));

      if(Sectvalue.freset.Tripdfdt.assessdfdttrip.relerror.valid==true&&\
         Sectvalue.freset.Tripdfdt.assessdfdttrip.abserror.valid!=true)
      {
          char tmpstr1[512];
          sprintf(tmpstr1,"Criterion=REL(%f)",Sectvalue.freset.Tripdfdt.assessdfdttrip.relerror.errorvalue);
          ui->textBrowser->append(tr(tmpstr1));
      }

      else if(Sectvalue.freset.Tripdfdt.assessdfdttrip.abserror.valid==true&&\
              Sectvalue.freset.Tripdfdt.assessdfdttrip.relerror.valid!=true)
       {
           char tmpstr1[512];
           sprintf(tmpstr1,"Criterion=ABS(%f,%f)",\
           Sectvalue.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue,\
           Sectvalue.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue_Negtive);
           ui->textBrowser->append(tr(tmpstr1));


       }
      else   if(Sectvalue.freset.Tripdfdt.assessdfdttrip.relerror.valid==true&&\
                Sectvalue.freset.Tripdfdt.assessdfdttrip.abserror.valid==true)
       {
           if(Sectvalue.freset.Tripdfdt.assessdfdttrip.assessandor==1) //与的关系
           {
               char tmpstr1[512];
               sprintf(tmpstr1,"Criterion=REL(%f)&ABS(%f,%f)",\
               Sectvalue.freset.Tripdfdt.assessdfdttrip.relerror.errorvalue,\
               Sectvalue.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue,\
               Sectvalue.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue_Negtive);
               ui->textBrowser->append(tr(tmpstr1));


           }
           else if(Sectvalue.freset.Tripdfdt.assessdfdttrip.assessandor==2)
           {
               char tmpstr1[512];
               sprintf(tmpstr1,"Criterion=REL(%f)|ABS(%f,%f)",\
               Sectvalue.freset.Tripdfdt.assessdfdttrip.relerror.errorvalue,\
               Sectvalue.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue,\
               Sectvalue.freset.Tripdfdt.assessdfdttrip.abserror.errorvalue_Negtive);
               ui->textBrowser->append(tr(tmpstr1));

           }

       }






  }
  else if(type==3)
  {
      ui->textBrowser->append(tr("Name:")+tr("df/dt blocking time"));
      ui->textBrowser->append(tr("Tripping value keeping(s):")+QString(Sectvalue.freset.Faultduraion.text));
      ui->textBrowser->append(tr("df/dt(Hz/s):")+QString(Sectvalue.freset.Tripdfdttime.dfdt.text));
      ui->textBrowser->append(tr("Frequency End(Hz):")+QString(Sectvalue.freset.Tripdfdttime.stopfreq.text));
      ui->textBrowser->append(tr("Duration(s):")+QString(Sectvalue.freset.Tripdfdttime.Duration.text));

      if(Sectvalue.freset.Tripdfdttime.assessdfdttriptime.relerror.valid==true&&\
         Sectvalue.freset.Tripdfdttime.assessdfdttriptime.abserror.valid!=true)
      {
          char tmpstr1[512];
          sprintf(tmpstr1,"Criterion=REL(%f)",\
          Sectvalue.freset.Tripdfdttime.assessdfdttriptime.relerror.errorvalue);
          ui->textBrowser->append(tr(tmpstr1));
      }
      else if(Sectvalue.freset.Tripdfdttime.assessdfdttriptime.relerror.valid!=true&&\
              Sectvalue.freset.Tripdfdttime.assessdfdttriptime.abserror.valid==true)
      {

          char tmpstr1[512];
          sprintf(tmpstr1,"Criterion=ABS(%f,%f)",\
          Sectvalue.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue,\
          Sectvalue.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue_Negtive);
          ui->textBrowser->append(tr(tmpstr1));
      }
      else if(Sectvalue.freset.Tripdfdttime.assessdfdttriptime.relerror.valid==true&&\
              Sectvalue.freset.Tripdfdttime.assessdfdttriptime.abserror.valid==true)
      {

          if(Sectvalue.freset.Tripdfdttime.assessdfdttriptime.assessandor==1) //与的关系
          {
              char tmpstr1[512];
              sprintf(tmpstr1,"Criterion=REL(%f)&ABS(%f,%f)",\
              Sectvalue.freset.Tripdfdttime.assessdfdttriptime.relerror.errorvalue,\
              Sectvalue.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue,\
              Sectvalue.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue_Negtive);
              ui->textBrowser->append(tr(tmpstr1));
          }
          else if(Sectvalue.freset.Tripdfdttime.assessdfdttriptime.assessandor==2)
          {
              char tmpstr1[512];
              sprintf(tmpstr1,"Criterion=REL(%f)|ABS(%f,%f)",\
              Sectvalue.freset.Tripdfdttime.assessdfdttriptime.relerror.errorvalue,\
              Sectvalue.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue,\
              Sectvalue.freset.Tripdfdttime.assessdfdttriptime.abserror.errorvalue_Negtive);
              ui->textBrowser->append(tr(tmpstr1));

          }

      }





  }
  else if(type==4)
  {

      ui->textBrowser->append(tr("Name:")+tr("Voltage blocking"));
      ui->textBrowser->append(tr("Tripping value keeping(s):")+QString(Sectvalue.freset.Faultduraion.text));
      ui->textBrowser->append(tr("df/dt(Hz/s):")+QString(Sectvalue.freset.TripVBlock.Dfdt.text));
      ui->textBrowser->append(tr("Frequency End(Hz):")+QString(Sectvalue.freset.TripVBlock.FreEndValue.text));
      ui->textBrowser->append(tr("Voltage Start(V):")+QString(Sectvalue.freset.TripVBlock.VStart.text));
      ui->textBrowser->append(tr("Voltage Stop(V):")+QString(Sectvalue.freset.TripVBlock.VEnd.text));
      ui->textBrowser->append(tr("Voltage Step(V):")+QString(Sectvalue.freset.TripVBlock.VStep.text));
      ui->textBrowser->append(tr("Hold Time(s):")+QString(Sectvalue.freset.TripVBlock.HoldTime.text));
      ui->textBrowser->append(tr("Hold Time(s):")+QString(Sectvalue.freset.TripVBlock.HoldTime.text));
      if(Sectvalue.freset.TripVBlock.VSelect==1)
      ui->textBrowser->append(tr("All Phase:ph-n"));
      else
      ui->textBrowser->append(tr("All Phase:ph-ph"));

      if(Sectvalue.freset.TripVBlock.assessvblock.relerror.valid==true\
       &&Sectvalue.freset.TripVBlock.assessvblock.abserror.valid!=true)
      {
          char tmpstr1[512];
          sprintf(tmpstr1,"Criterion=REL(%f)",\
          Sectvalue.freset.TripVBlock.assessvblock.relerror.errorvalue);
          ui->textBrowser->append(tr(tmpstr1));
      }

     else if(Sectvalue.freset.TripVBlock.assessvblock.abserror.valid==true\
           &&Sectvalue.freset.TripVBlock.assessvblock.relerror.valid!=true)
      {
          char tmpstr1[512];
          sprintf(tmpstr1,"Criterion=ABS(%f,%f)",\
          Sectvalue.freset.TripVBlock.assessvblock.abserror.errorvalue,\
          Sectvalue.freset.TripVBlock.assessvblock.abserror.errorvalue_Negtive);
          ui->textBrowser->append(tr(tmpstr1));
      }
     else if(Sectvalue.freset.TripVBlock.assessvblock.relerror.valid==true&&\
             Sectvalue.freset.TripVBlock.assessvblock.abserror.valid==true)
      {

         if(Sectvalue.freset.TripVBlock.assessvblock.assessandor==1) //与的关系
         {
               char tmpstr1[512];
               sprintf(tmpstr1,"Criterion=REL(%f)&ABS(%f,%f)",\
               Sectvalue.freset.TripVBlock.assessvblock.relerror.errorvalue,\
               Sectvalue.freset.TripVBlock.assessvblock.abserror.errorvalue,\
               Sectvalue.freset.TripVBlock.assessvblock.abserror.errorvalue_Negtive);
               ui->textBrowser->append(tr(tmpstr1));
         }
         else if(Sectvalue.freset.TripVBlock.assessvblock.assessandor==2)
         {
             char tmpstr1[512];
             sprintf(tmpstr1,"Criterion=REL(%f)|ABS(%f,%f)",\
             Sectvalue.freset.TripVBlock.assessvblock.relerror.errorvalue,\
             Sectvalue.freset.TripVBlock.assessvblock.abserror.errorvalue,\
             Sectvalue.freset.TripVBlock.assessvblock.abserror.errorvalue_Negtive);

            ui->textBrowser->append(tr(tmpstr1));
         }
      }








  }





}

void ShowTestitemPara::ShowPhasePara(int type)
{
    if(type==0) //pick up
     {
      ui->textBrowser->append(tr("Name:")+tr("Pick up"));
      char ChangeItem[512]="Variable:";
      if(Sectvalue.phraseset.VAch==1)
          strcat(ChangeItem,"VA ");
      if(Sectvalue.phraseset.VBch==1)
          strcat(ChangeItem,"VB ");
      if(Sectvalue.phraseset.VCch==1)
          strcat(ChangeItem,"VC ");

      if(Sectvalue.phraseset.IAch==1)
          strcat(ChangeItem,"IA ");
      if(Sectvalue.phraseset.IBch==1)
          strcat(ChangeItem,"IB ");
      if(Sectvalue.phraseset.ICch==1)
          strcat(ChangeItem,"IC ");
      ui->textBrowser->append(tr(ChangeItem));
      ui->textBrowser->append(tr("Tripping value keeping(s):")+QString(Sectvalue.phraseset.Faultduraion.text));
      ui->textBrowser->append(tr("Fix Voltage(V):")+QString(Sectvalue.phraseset.FixVoltage.text));
      ui->textBrowser->append(tr("Fix Current(A):")+QString(Sectvalue.phraseset.FixCurrent.text));


      ui->textBrowser->append(tr("Start Phase(Deg):")+QString(Sectvalue.phraseset.TripValue.StartValue.text));
      ui->textBrowser->append(tr("End Phase(Deg):")+QString(Sectvalue.phraseset.TripValue.EndValue.text));
      ui->textBrowser->append(tr("Phase Step(Deg):")+QString(Sectvalue.phraseset.TripValue.StepValue.text));
      ui->textBrowser->append(tr("Hold Time(s):")+QString(Sectvalue.phraseset.TripValue.HoldTime.text));


      if(Sectvalue.phraseset.TripValue.assesstripvalue.relerror.valid==true\
              &&Sectvalue.phraseset.TripValue.assesstripvalue.abserror.valid!=true)
      {
          char tmpstr1[512];
          sprintf(tmpstr1,"Criterion=REL(%f)",Sectvalue.phraseset.TripValue.assesstripvalue.relerror.errorvalue);
          ui->textBrowser->append(tr(tmpstr1));
      }

      else if(Sectvalue.phraseset.TripValue.assesstripvalue.abserror.valid==true\
              &&Sectvalue.phraseset.TripValue.assesstripvalue.relerror.valid!=true)
      {
          char tmpstr1[512];
          sprintf(tmpstr1,"Criterion=ABS(%f,%f)",\
          Sectvalue.phraseset.TripValue.assesstripvalue.abserror.errorvalue,\
          Sectvalue.phraseset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);
          ui->textBrowser->append(tr(tmpstr1));

      }


      else  if(Sectvalue.phraseset.TripValue.assesstripvalue.relerror.valid==true&&\
      Sectvalue.phraseset.TripValue.assesstripvalue.abserror.valid==true)
      {

          if(Sectvalue.phraseset.TripValue.assesstripvalue.assessandor==1) //与的关系
          {
               char tmpstr1[512];
               sprintf(tmpstr1,"Criterion=REL(%f)&ABS(%f,%f)",\
               Sectvalue.phraseset.TripValue.assesstripvalue.relerror.errorvalue,\
               Sectvalue.phraseset.TripValue.assesstripvalue.abserror.errorvalue,\
               Sectvalue.phraseset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);
               ui->textBrowser->append(tr(tmpstr1));

          }
          else if(Sectvalue.phraseset.TripValue.assesstripvalue.assessandor==2)
          {
              char tmpstr1[512];
               sprintf(tmpstr1,"Criterion=REL(%f)|ABS(%f,%f)",\
               Sectvalue.phraseset.TripValue.assesstripvalue.relerror.errorvalue,\
               Sectvalue.phraseset.TripValue.assesstripvalue.abserror.errorvalue,\
               Sectvalue.phraseset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);
               ui->textBrowser->append(tr(tmpstr1));
          }
      }




     }
    else if(type==1) //
    {
        if(Sectvalue.phraseset.TripTime.assesstriptime.abserror.errorvalue==-1&&\
           Sectvalue.phraseset.TripTime.assesstriptime.abserror.errorvalue_Negtive==-1)
        ui->textBrowser->append(tr("Name:")+tr("Phase Stability"));
        else
        ui->textBrowser->append(tr("Name:")+tr("Trip time"));

        char ChangeItem[512]="Variable:";
        if(Sectvalue.phraseset.VAch==1)
            strcat(ChangeItem,"VA ");
        if(Sectvalue.phraseset.VBch==1)
            strcat(ChangeItem,"VB ");
        if(Sectvalue.phraseset.VCch==1)
            strcat(ChangeItem,"VC ");

        if(Sectvalue.phraseset.IAch==1)
            strcat(ChangeItem,"IA ");
        if(Sectvalue.phraseset.IBch==1)
            strcat(ChangeItem,"IB ");
        if(Sectvalue.phraseset.ICch==1)
            strcat(ChangeItem,"IC ");
        ui->textBrowser->append(tr(ChangeItem));
        ui->textBrowser->append(tr("Tripping value keeping(s):")+QString(Sectvalue.phraseset.Faultduraion.text));
        ui->textBrowser->append(tr("Fix Voltage(V):")+QString(Sectvalue.phraseset.FixVoltage.text));
        ui->textBrowser->append(tr("Fix Current(A):")+QString(Sectvalue.phraseset.FixCurrent.text));

        ui->textBrowser->append(tr("Phase End(A):")+QString(Sectvalue.phraseset.TripTime.EndVaule.text));
        ui->textBrowser->append(tr("Hold Time(s):")+QString(Sectvalue.phraseset.TripTime.HoldTime.text));
        bool Showassess=(Sectvalue.phraseset.TripTime.assesstriptime.abserror.errorvalue==-1&&\
                           Sectvalue.phraseset.TripTime.assesstriptime.abserror.errorvalue_Negtive==-1);
        if(Showassess==false)
        {

            if(Sectvalue.phraseset.TripTime.assesstriptime.relerror.valid==true&&Sectvalue.phraseset.TripTime.assesstriptime.abserror.valid!=true)
            {
                char tmpstr1[512];
                sprintf(tmpstr1,"Criterion=REL(%f)",Sectvalue.phraseset.TripTime.assesstriptime.relerror.errorvalue);
                ui->textBrowser->append(tr(tmpstr1));
            }

           else if(Sectvalue.phraseset.TripTime.assesstriptime.abserror.valid==true&&Sectvalue.phraseset.TripTime.assesstriptime.relerror.valid!=true)
            {
                char tmpstr1[512];
                sprintf(tmpstr1,"Criterion=ABS(%f,%f)",\
                Sectvalue.phraseset.TripTime.assesstriptime.abserror.errorvalue,\
                Sectvalue.phraseset.TripTime.assesstriptime.abserror.errorvalue_Negtive);

                ui->textBrowser->append(tr(tmpstr1));

            }
           else  if(Sectvalue.phraseset.TripTime.assesstriptime.relerror.valid==true&&\
               Sectvalue.phraseset.TripTime.assesstriptime.abserror.valid==true)
            {

                if(Sectvalue.phraseset.TripTime.assesstriptime.assessandor==1) //与的关系
                {
                     char tmpstr1[512];
                     sprintf(tmpstr1,"Criterion=REL(%f)&ABS(%f,%f)",\
                     Sectvalue.phraseset.TripTime.assesstriptime.relerror.errorvalue,\
                     Sectvalue.phraseset.TripTime.assesstriptime.abserror.errorvalue,\
                     Sectvalue.phraseset.TripTime.assesstriptime.abserror.errorvalue_Negtive);
                     ui->textBrowser->append(tr(tmpstr1));


                }
                else if(Sectvalue.phraseset.TripTime.assesstriptime.assessandor==2)
                {
                     char tmpstr1[512];
                    sprintf(tmpstr1,"Criterion=REL(%f)|ABS(%f,%f)",\
                    Sectvalue.phraseset.TripTime.assesstriptime.relerror.errorvalue,\
                    Sectvalue.phraseset.TripTime.assesstriptime.abserror.errorvalue,\
                    Sectvalue.phraseset.TripTime.assesstriptime.abserror.errorvalue_Negtive);
                    ui->textBrowser->append(tr(tmpstr1));
                }
            }



        }


    }

}

void ShowTestitemPara::ShowHarmonicPara()
{


    ui->textBrowser->append(tr("Name:")+tr("Pick up"));

    char ChangeItem[512]="Variable:";
    if(Sectvalue.harmonicset.IAch==1)
        strcat(ChangeItem,"IA ");
    if(Sectvalue.harmonicset.IBch==1)
        strcat(ChangeItem,"IB ");
    if(Sectvalue.harmonicset.ICch==1)
        strcat(ChangeItem,"IC ");
    ui->textBrowser->append(tr(ChangeItem));
    ui->textBrowser->append(tr("Fundamental wave Current(A):")+tr(Sectvalue.harmonicset.Current.text));
    ui->textBrowser->append(tr("Fundamental wave Phase(°):")+tr(Sectvalue.harmonicset.Phase.text));
    ui->textBrowser->append(tr("Harmonic Number:")+tr(Sectvalue.harmonicset.HarmonicCount.text));


    ui->textBrowser->append(tr("Start Harmonic(%):")+QString(Sectvalue.harmonicset.TripValue.StartValue.text));
    ui->textBrowser->append(tr("End Harmonic(%):")+QString(Sectvalue.harmonicset.TripValue.EndValue.text));
    ui->textBrowser->append(tr("Harmonic Step(Deg):")+QString(Sectvalue.harmonicset.TripValue.StepValue.text));
    ui->textBrowser->append(tr("Hold Time(s):")+QString(Sectvalue.harmonicset.TripValue.HoldTime.text));


    if(Sectvalue.harmonicset.TripValue.assesstripvalue.relerror.valid==true\
            &&Sectvalue.harmonicset.TripValue.assesstripvalue.abserror.valid!=true)
    {
        char tmpstr1[512];
        sprintf(tmpstr1,"Criterion=REL(%f)",Sectvalue.harmonicset.TripValue.assesstripvalue.relerror.errorvalue);
        ui->textBrowser->append(tr(tmpstr1));
    }

    else if(Sectvalue.harmonicset.TripValue.assesstripvalue.abserror.valid==true\
            &&Sectvalue.harmonicset.TripValue.assesstripvalue.relerror.valid!=true)
    {
        char tmpstr1[512];
        sprintf(tmpstr1,"Criterion=ABS(%f,%f)",\
        Sectvalue.harmonicset.TripValue.assesstripvalue.abserror.errorvalue,\
        Sectvalue.harmonicset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);
        ui->textBrowser->append(tr(tmpstr1));

    }


    else  if(Sectvalue.harmonicset.TripValue.assesstripvalue.relerror.valid==true&&\
    Sectvalue.harmonicset.TripValue.assesstripvalue.abserror.valid==true)
    {

        if(Sectvalue.harmonicset.TripValue.assesstripvalue.assessandor==1) //与的关系
        {
             char tmpstr1[512];
             sprintf(tmpstr1,"Criterion=REL(%f)&ABS(%f,%f)",\
             Sectvalue.harmonicset.TripValue.assesstripvalue.relerror.errorvalue,\
             Sectvalue.harmonicset.TripValue.assesstripvalue.abserror.errorvalue,\
             Sectvalue.harmonicset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);
             ui->textBrowser->append(tr(tmpstr1));

        }
        else if(Sectvalue.harmonicset.TripValue.assesstripvalue.assessandor==2)
        {
            char tmpstr1[512];
             sprintf(tmpstr1,"Criterion=REL(%f)|ABS(%f,%f)",\
             Sectvalue.harmonicset.TripValue.assesstripvalue.relerror.errorvalue,\
             Sectvalue.harmonicset.TripValue.assesstripvalue.abserror.errorvalue,\
             Sectvalue.harmonicset.TripValue.assesstripvalue.abserror.errorvalue_Negtive);
             ui->textBrowser->append(tr(tmpstr1));
        }
    }




}


void ShowTestitemPara::on_pushButton_clicked()  //关闭
{
 emit sendflag();
 close();
}

void ShowTestitemPara::closeEvent(QCloseEvent *)
{

 emit sendflag();
 close();
}
