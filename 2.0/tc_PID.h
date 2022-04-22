#ifndef PID_H
#define PID_H
#include "tc_Configuration.h"
#define LIBRARY_VERSION        1.0.0

class PID
{


  public:

    //Constants used in some of the functions below                 閸︺劋绗呴棃銏犲毐閺侀鑵戞担璺ㄦ暏閻ㄥ嫬鐖堕弫锟�

    //commonly used functions ************************************************************************  闁艾鐖舵担璺ㄦ暏閻ㄥ嫬鍤遍弫锟�
    PID(double*, double*, double*,        // * constructor.  links the PID to the Input, Output, and           閺嬪嫰锟界姴鍤遍弫鑸拷鍌濈箾閹侯櫀ID閻ㄥ嫯绶崗銉ｏ拷浣界翻閸戝搫鎷扮拋鍓х枂閻愬箍锟斤拷
        double, double, double, int);     //   Setpoint.  Initial tuning parameters are also set here                        閸掓繂顫愰崠鏍殶娴兼ê寮弫鑸拷锟�

    void SetMode(int Mode);               // * sets PID to either Manual (0) or Auto (non-0)                                                 鐠佸墽鐤哖ID娑撻缚鍤滈崝銊﹀灗閼板懏澧滈崝銊δ佸蹇嬶拷锟�

    bool Compute();                       // * performs the PID calculation.  it should be                                                                         閹笛嗩攽PID鐠侊紕鐣婚妴锟�
    //   called every time loop() cycles. ON/OFF and                                                                                                 鐎瑰啫褰叉禒銉ユ躬濮ｅ繑顐糽oop瀵邦亞骞嗛惃鍕閸婃瑨鐨熼悽銊ｏ拷锟�
    //   calculation frequency can be set using SetMode                                                                                         瀵拷閸忓磭濮搁幀浣告嫲鐠侊紕鐣绘０鎴犲芳閸欘垯浜掗悽鈯縠tMode閸滃etSanmpleTime閸掑棗鍩嗛崷鎷岊啎缂冾喓锟斤拷
    //   SetSampleTime respectively

    void SetOutputLimits(double, double); //clamps the output to a specific range. 0-255 by default, but                闂勬劕鍩楁潏鎾冲毉閸︺劋绔存稉顏嗗鐎规氨娈戦懠鍐ㄦ纯閵嗗倿绮拋銈嗘Ц0-255閿涳拷
    //it's likely the user will want to change this depending on                                                                                娴ｅ棙妲搁悽銊﹀煕閸欘垯浜掗弽瑙勫祦鐎圭偤妾惔鏃傛暏閺�鐟板綁鏉╂瑩鍣烽妴锟�
    //the application

    //  void Steadycharge( );
    //  void Shake();
    //  void Autotune();
    //  void change();
    void init_output();
    //available but not commonly used functions ********************************************************         閸欘垯浜掓担璺ㄦ暏娴ｅ棝锟芥艾鐖舵稉宥囨暏閻ㄥ嫬鍤遍弫锟�
    void SetTunings(double, double,       // * While most users will set the tunings once in the                                  瑜版挸銇囨径姘殶閻€劍鍩涢崷銊︾�柅鐘插毐閺侀鑵戠拋鍓х枂娑擄拷濞喡ょ殶閼哄倸寮弫甯礉
                    double);                   //   constructor, this function gives the user the option                                                                 鏉╂瑤閲滈崙鑺ユ殶缂佹瑧鏁ら幋铚傜娑擃亜婀潻鎰攽鏉╁洨鈻兼稉顓熸暭閸欐鐨熼懞鍌氬棘閺佹壆娈戦柅澶嬪
    //   of changing tunings during runtime for Adaptive control
    void SetControllerDirection(int);          // * Sets the Direction, or "Action" of the controller. DIRECT                 鐠佸墽鐤嗛弬鐟版倻閿涘本鍨ㄩ懓鍛邦嚛閹貉冨煑閻ㄥ嫬濮╂担婧匡拷锟�
    //   means the output will increase when error is positive. REVERSE                                 Direct閹板繐鎳楅惈锟借ぐ鎾诡嚖瀹割喕璐熷锝嗘閿涘矁绶崙鍝勵杻閸旂姰锟戒縼everse閸欏秳绠ｉ妴锟�
    //   means the opposite.  it's very unlikely that this will be needed                                         鏉╂瑩锟芥艾鐖堕崣顏勬躬閺嬪嫰锟界姴鍤遍弫棰佽厬鐠佸墽鐤嗘稉锟藉▎掳锟斤拷
    //   once it is set in the constructor.


  private:
    void Initialize();

    double kp;                  // * (P)roportional Tuning Parameter                                                                                                                                           P閸欏倹鏆�
    double ki;                  // * (I)ntegral Tuning Parameter                                                                                                                                                           I 閸欏倹鏆�
    double kd;                  // * (D)erivative Tuning Parameter                                                                                                                                                  D閸欏倹鏆�

    int controllerDirection;

    double *myInput;              // * Pointers to the Input, Output, and Setpoint variables                                                               鏉堟挸鍙嗛妴浣界翻閸戝搫鎷扮拋鍓х枂閻愮懓褰夐柌蹇曟畱閹稿洭鎷�
    double *myOutput;             //   This creates a hard link between the variables and the                                                           鏉╂瑧鏁撻幋鎰娑擃亞鈥栭柧鐐复閿涘苯婀崣姗�鍣洪崪瀛璉D娑斿妫�
    double *mySetpoint;           //   PID, freeing the user from having to constantly tell us                                                           娴犲簼绗夊妞剧瑝閹镐胶鐢婚崨濠呯様閹存垳婊戞潻娆庣昂閸婂吋妲告禒锟芥稊鍫滆厬閿涘矁袙閺�鍙ョ啊閻€劍鍩涢敍灞惧灉娴狀剙褰ч棁锟界憰浣瑰瘹闁斤拷
    //   what these values are.  with pointers we'll just know.

    double charge = 0;
    //       double inputHighAll=0;
    //       double inputLowAll=0;
    //       int inputHighNum=0;
    //       int inputLowNum=0;
    //       double inputHistory[3];
    //       int j=0;
    //       int i=200;
    //       int flag=0;
    //       int atemp=1;
    //       int btemp=1;
    //       int  count=0;
    //       int Steady=0;
    double ITerm, lastInput;

    unsigned long SampleTime;
    double outMin, outMax;
    bool inAuto;
};
#endif
