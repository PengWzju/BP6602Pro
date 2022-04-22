/**********************************************************************************************
  Arduino PID Library - Version 1.0.1
  by Brett Beauregard <[url=mailto:br3ttb@gmail.com]br3ttb@gmail.com[/url]> brettbeauregard.com

  This Library is licensed under a GPLv3 License
**********************************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "tc_PID.h"
#include "tc_configuration.h"

#define AUTOMATIC  1
#define MANUAL 0

/*Constructor (...)*********************************************************                                閺嬪嫰锟界姴鍤遍弫锟�
      The parameters specified here are those for for which we can't set up                                 鏉╂瑩鍣烽幐鍥х暰閻ㄥ嫬寮弫鐗堟Ц闁絼绨洪幋鎴滄粦娑撳秷鍏樺铏圭彌閸欘垶娼惃鍕暕鐠佹儳锟界》绱�
      reliable defaults, so we need to have the user set them.                                                                                閸ョ姵顒濋幋鎴滄粦闂囷拷鐟曚焦婀侀悽銊﹀煕閺夈儴顔曠純顔肩暊娴狀兙锟斤拷
 ***************************************************************************/
PID::PID(double* Input, double* Output, double* Setpoint,
         double Kp, double Ki, double Kd, int ControllerDirection)
{
  myOutput = Output;
  myInput = Input;
  mySetpoint = Setpoint;
  inAuto = false;

  // PID::SetOutputLimits(0, 255);                                //default output limit corresponds to                                         姒涙顓绘潏鎾冲毉闂勬劕鍩楅敍灞炬Ц閻㈢泧rduino PWM闂勬劕鍩楅惃锟�
  //the arduino pwm limits

  SampleTime = SAMPLE_TIME / 1000;                                                      //default Controller Sample Time is 0.1 seconds                姒涙顓婚幒褍鍩楅柌鍥ㄧ壉閺冨爼妫�0.1s

  PID::SetControllerDirection(ControllerDirection);
  PID::SetTunings(Kp, Ki, Kd);
}


/* Compute() **********************************************************************
       This, as they say, is where the magic happens.  this function should be called
     every time "void loop()" executes.  the function will decide for itself whether a new
     pid Output needs to be computed.  returns true when the output is computed,
     false when nothing has been done.

      鐠侊紕鐣�
     鏉╂瑤閲滈崙鑺ユ殶閸︺劍鐦″▎鈩冨⒔鐞涘oop閻ㄥ嫭妞傞崐娆掔殶閻€劊锟藉倽绻栨稉顏勫毐閺佹澘鐨㈤崘鍐茬暰閺勵垰鎯佺拋锛勭暬娑擄拷娑擃亝鏌婇惃鍑盜D鏉堟挸鍤妴锟�
     瑜版捁绶崙楦款吀缁犳鐣幋鎰倵鏉╂柨娲栭惇鐕傜礉瑜版挷绮堟稊鍫ュ厴濞屸�崇暚閹存劕鎮楁潻鏂挎礀閸嬪洢锟斤拷
 **********************************************************************************/
bool PID::Compute()
{
  //      /*Compute all the working error variables*/                                        // 鐠侊紕鐣婚幍锟介張澶屾畱瀹搞儰缍旂拠顖氭▕閸欐﹢鍣�
  double input = *myInput;
  double error = *mySetpoint - input;                                               // error=鐠佹儳鐣鹃悙锟�-鏉堟挸鍙嗛崐锟�    娓氬顩ч幒褎淇化鑽ょ埠娑擃叏绱濊ぐ鎾翠刊鎼达箑鐨禍搴ゎ啎鐎规氨鍋ｉ敍瀹攔ror娑撶儤顒�
  ITerm += (ki * error);
  if (ITerm > outMax) ITerm = outMax;
  else if (ITerm < outMin) ITerm = outMin;
  double dInput = (input - lastInput);

  /*Compute PID Output*/                                                                                                        // 鐠侊紕鐣籔ID鏉堟挸鍤�
  double output = kp * error + ITerm - kd * dInput;
  // Serial.print(output);
  if (output > outMax) output = outMax;
  else if (output < outMin) output = outMin;
  *myOutput = output;
  /*Remember some variables for next time*/                                     // 娑撹桨绗呭▎陇顓哥粻妤勵唶瑜版洑绔存禍娑樺綁闁诧拷
  lastInput = input;
  //      if(charge==0){
  //          PID::Steadycharge();
  //      }
  //      else if(charge==1){
  //        PID::Shake();
  //      }
  //      else if(charge==2){
  //        PID::Autotune();
  //      }
  return true;
}

/*閸掋倖鏌囬弰顖氭儊鏉堟儳鍩岀粙鍐茬暰*/
//void PID::Steadycharge( )
//{
//     inputHistory[2] = *myInput;
//     if(Steady==3){
//        flag=1;
//     }
//     else{
//       if (inputHistory[1] > inputHistory[0] && inputHistory[1] > inputHistory[2]){
//           Steady=Steady+1;
//       }
//       inputHistory[0]=inputHistory[1];
//       inputHistory[1]=inputHistory[2];
//     }
//     if(flag==1){
//          count=count+1;
//     }
//     if(count==50){
//        count=0;
//        flag=0;
//        Steady=0;
//        charge=1;
//     }
//}

//void PID::Shake(){
//  i=i-1;
//  j=j+1;
//  if(controllerDirection ==REVERSE){
//        if(*myInput>*mySetpoint){
//           *myOutput==outMax;
//        }else
//        {
//           *myOutput==outMin;
//        }
//  }
//  else{
//    if(*myInput<*mySetpoint){
//           *myOutput==outMax;
//        }else
//        {
//           *myOutput==outMin;
//        }
//  }
//  if(i<0){
//      inputHistory[2] = *myInput;
//      if (inputHistory[1] > inputHistory[0] && inputHistory[1] > inputHistory[2]){
//        inputHighAll =inputHighAll+ inputHistory[1];                                            //濞夈垹鍢查崐鍏硷拷缁樻殶婢х偛濮�
//        inputHighNum=inputHighNum+1;
//        if (inputHighNum == 1) atemp = j-1;
//        btemp =j-1;
//      }
//      else if (inputHistory[1] < inputHistory[0] && inputHistory[1] < inputHistory[2]){//婵″倹鐏夐弰顖涘皾鐠嬪嚖绱濈猾璁虫妧娑撳﹪娼�
//                    inputLowAll =inputLowAll+ inputHistory[1];
//                    inputLowNum=inputLowNum+1;
//    }
//    inputHistory[0]=inputHistory[1];
//    inputHistory[1]=inputHistory[2];
//  }
//  if(inputHighNum==3){
//    charge=2;
//    i=200;
//    j=0;
//  }
//}
//void PID::Autotune(){
//            double outputStep=outMax/2;
//            float autoTuneA = (inputHighAll / inputHighNum) - (inputLowAll / inputLowNum);                  //瀹勬澘鍢查崐鑲╂畱楠炲啿娼庨弫鐧�
//            float autoTunePu = (btemp - atemp) * 0.1 / (inputHighNum - 1);                                  //娑撱倓閲滃▔銏犲槻娑斿妫块惃鍕闂傛挳妫块梾鎿痷(s)
//            double Ku = 4 * outputStep / (autoTuneA * 3.14159);
//            double Kp = 0.6 * Ku;
//            double Ki = 1.2 * Ku / autoTunePu;
//            double Kd = 0.075*Ku*autoTunePu;
//             PID::SetTunings(Kp, Ki, Kd);
//            charge=3;
//}
/* SetTunings(...)*************************************************************
   This function allows the controller's dynamic performance to be adjusted.
   it's called automatically from the constructor, but tunings can also
   be adjusted on the fly during normal operation
   鐠佸墽鐤嗙拫鍐Ν閸欏倹鏆�
   鏉╂瑤閲滈崙鑺ユ殶閸忎浇顔忕亸鍡樺付閸掕泛娅掗惃鍕З閹焦锟窖嗗厴鐞氼偉鐨熼弫娣拷锟�
   鐎瑰啩绮犻弸鍕拷鐘插毐閺侀鑵戠悮顐ュ殰閸斻劌婀寸拫鍐暏閿涘奔绲鹃弰顖濈殶閼哄倷绡冮崣顖欎簰閸︺劍顒滅敮姝岀箥鐞涘本婀￠梻鏉戝З閹礁婀寸拫鍐╂殻閵嗭拷
 ******************************************************************************/
void PID::SetTunings(double Kp, double Ki, double Kd)
{
  if (Kp < 0 || Ki < 0 || Kd < 0) return;
  double SampleTimeInSec = ((double)SampleTime) / 1000;
  kp = Kp;
  ki = Ki * SampleTimeInSec;
  kd = Kd / SampleTimeInSec;

  if (controllerDirection == REVERSE)
  {
    kp = (0 - kp);
    ki = (0 - ki);
    kd = (0 - kd);
  }
}
//void PID::change(){
//     charge=0;
//     inputHighNum=0;
//     inputLowNum=0;
//     inputLowAll=0;
//     inputHighAll=0;
//}


/* SetSampleTime(...) *********************************************************
   sets the period, in Milliseconds, at which the calculation is performed
   鐠佸墽鐤嗛柌鍥ㄧ壉閺冨爼妫�
   鐠佸墽鐤嗙拋锛勭暬閹笛嗩攽閻ㄥ嫬鎳嗛張鐕傜礉娴狀櫝s娑撳搫宕熸担宥冿拷锟�
 ******************************************************************************/


/* SetOutputLimits(...)****************************************************
       This function will be used far more often than SetInputLimits.  while
    the input to the controller will generally be in the 0-1023 range (which is
    the default already,)  the output will be a little different.  maybe they'll
    be doing a time window and will need 0-8000 or something.  or maybe they'll
    want to clamp it from 0-125.  who knows.  at any rate, that can all be done
    here.
   鐠佸墽鐤嗘潏鎾冲毉闂勬劕鍩�
   鏉╂瑤閲滈崙鑺ユ殶鐏忓棙鐦甋etInputLimits閸戣姤鏆熼弴鏉戠埗閻€劊锟藉倸缍嬮幒褍鍩楅崳銊ㄧ翻閸忋儳鏁撻幋鎰煂0-1023閻ㄥ嫯瀵栭崶杈剧礄瀹歌尙绮￠弰顖炵帛鐠併倧绱氶敍锟�
   鏉堟挸鍤亸鍡樻箒娑擄拷閻愰�涚瑝閸氬被锟藉倸褰查懗钘夌暊娴狀剙鐨㈤幍褑顢戞稉锟芥稉顏呮闂傚鐛ラ崣锝忕礉閹存牞锟藉懘娓剁憰锟�0-8000缁涘锟斤拷
   閹存牞锟藉懎鐣犳禒顒�褰查懗鑺ュ厒鐟曚線妾洪崚璺烘躬0-125. 閺冪姾顔戞俊鍌欑秿閿涘矂鍏橀崣顖欎簰閸︺劏绻栭柌宀冾啎缂冾喓锟斤拷
 **************************************************************************/
void PID::SetOutputLimits(double Min, double Max)
{
  if (Min >= Max) return;
  outMin = Min;
  outMax = Max;
  if (*myOutput > outMax) *myOutput = outMax;
  else if (*myOutput < outMin) *myOutput = outMin;
  if (ITerm > outMax) ITerm = outMax;
  else if (ITerm < outMin) ITerm = outMin;
}

/* SetMode(...)****************************************************************
   Allows the controller Mode to be set to manual (0) or Automatic (non-zero)
   when the transition from manual to auto occurs, the controller is
   automatically initialized
   鐠佸墽鐤嗗Ο鈥崇础
   閸忎浇顔忛幒褍鍩楅崳銊δ佸蹇氼啎鐎规碍鍨氶幍瀣З閿涳拷0閿涘鎷伴懛顏勫З閿涘牓娼�0閿涘锟斤拷
   瑜版挷绮犻幍瀣З鏉烆剚鍨氶懛顏勫З閿涘本甯堕崚璺烘珤閼奉亜濮╅崚婵嗩潗閸栵拷
 ******************************************************************************/
void PID::SetMode(int Mode)
{
  bool newAuto = (Mode == AUTOMATIC);
  if (newAuto == !inAuto)
  { /*we just went from manual to auto     閹存垳婊戦崣顏呮Ц娴犲孩澧滈崝銊ュ煂閼奉亜濮�*/
    PID::Initialize();
  }
  inAuto = newAuto;
}

/* Initialize()****************************************************************
          does all the things that need to happen to ensure a bumpless transfer
    from manual to automatic mode.
   閸掓繂顫愰崠锟�
   閸嬫碍澧嶉張澶愭付鐟曚礁褰傞悽鐔烘畱娴滃鍎忔禒銉ょ箽鐠囦椒绮犻幍瀣З閸掓媽鍤滈崝銊︽￥閹垫澘濮╂潪顒傂╅妴锟�
 ******************************************************************************/
void PID::Initialize()
{
  ITerm = *myOutput;
  lastInput = *myInput;
  if (ITerm > outMax) ITerm = outMax;
  else if (ITerm < outMin) ITerm = outMin;
//  Serial.print("ITerm:");
//  Serial.print(ITerm);
//  Serial.println();
}

/* SetControllerDirection(...)*************************************************
   The PID will either be connected to a DIRECT acting process (+Output leads
   to +Input) or a REVERSE acting process(+Output leads to -Input.)  we need to
   know which one, because otherwise we may increase the output when we should
   be decreasing.  This is called from the constructor.
   鐠佸墽鐤嗛幒褍鍩楅弬鐟版倻
   PID閸欘垯浜掓潻鐐村复閸掔増顒滈崥鎴滅稊閻€劏绻冪粙瀣剁礄濮濓綀绶崙鍝勵嚤閼峰瓨顒滄潏鎾冲弳閿涘鎷伴崣宥呮倻娴ｆ粎鏁ゆ潻鍥┾柤閿涘牊顒滄潏鎾冲毉鐎佃壈鍤х拹鐔荤翻閸忋儻绱�
   閹存垳婊戦棁锟界憰浣虹叀闁挻妲搁崫顏冮嚋閿涘苯鎯侀崚娆愬灉娴狀剙褰查懗钘夋躬鎼存棁顕氶崙蹇撶毈閺冭泛顤冮崝鐘虹翻閸戞亽锟藉倽绻栭弰顖欑矤閺嬪嫰锟界姴鍤遍弫鎷岀殶閻€劎娈戦妴锟�
 ******************************************************************************/
void PID::SetControllerDirection(int Direction)
{
  controllerDirection = Direction;
  PID::init_output();
}

void PID::init_output()
{
  PID::Initialize();
}

/* Status Funcions*************************************************************
   Just because you set the Kp=-1 doesn't mean it actually happened.  these
   functions query the internal state of the PID.  they're here for display
   purposes.  this are the functions the PID Front-end uses for example
   閻樿埖锟戒礁鍤遍弫锟�
   娴犲懍绮庨崶鐘辫礋娴ｇ姾顔曠純顔荤啊Kp = 1楠炴湹绗夐幇蹇撴嚄閻拷鐎瑰啰婀￠惃鍕絺閻㈢喍绨￠妴锟�
   閸戣姤鏆熼弻銉嚄PID閻ㄥ嫬鍞撮柈銊уЦ閹降锟藉倸鐣犳禒顒�婀潻娆撳櫡閺勵垯璐熸禍鍡樻▔缁�铏规畱閻╊喚娈戦妴锟�
   鏉╂瑦妲窹ID閸撳秶顏担璺ㄦ暏閻ㄥ嫬濮涢懗濮愶拷锟�
 ******************************************************************************/
