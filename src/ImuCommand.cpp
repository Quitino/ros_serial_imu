#include"ImuCommand.h"



bool IPSG::CImuCommand::display_decodeData(double *tmpBuffer)
{
    for(int i = 0;i < DATA_LENGTH;i ++)
    {
        std::cout<<tmpBuffer[i]<<std::endl;
    }
    return true;
}

bool IPSG::CImuCommand::display_Q4decodeData(double tmpBuffer[Q4DATA_LENGTH])
{
    for(int i = 0;i < Q4DATA_LENGTH;i ++)
    {
        std::cout<<tmpBuffer[i]<<std::endl;
    }
    return true;
}

bool IPSG::CImuCommand::display_Imumsg(sensor_msgs::Imu imumsg)
{   
    std::cout<<"print Q4 data!"<<std::endl;
    std::cout<<imumsg.orientation.x<<std::endl;
    std::cout<<imumsg.orientation.y<<std::endl;
    std::cout<<imumsg.orientation.z<<std::endl;
    std::cout<<imumsg.orientation.w<<std::endl;

    std::cout<<"print GRY data!"<<std::endl;
    std::cout<<imumsg.angular_velocity.x<<std::endl;
    std::cout<<imumsg.angular_velocity.y<<std::endl;
    std::cout<<imumsg.angular_velocity.z<<std::endl;

    std::cout<<"print ACC data!"<<std::endl;
    std::cout<<imumsg.linear_acceleration.x<<std::endl;
    std::cout<<imumsg.linear_acceleration.y<<std::endl;
    std::cout<<imumsg.linear_acceleration.z<<std::endl;
}

bool IPSG::CImuCommand::decodeFrame(unsigned char tmpBuffer[READ_BUFFERSIZE])
{
    if(tmpBuffer[0] == DATA_FrameHead && tmpBuffer[1] == DATA_FrameHead)
    {
        switch(tmpBuffer[2])
        {
            case DATA_TYPE_Q4:  //该帧输出数据为四元数类型
                { 
                    Q4[0] = (double)(tmpBuffer[4]<<8|tmpBuffer[5])/Q4_MEASURE;
                    Q4[1] = (double)(tmpBuffer[6]<<8|tmpBuffer[7])/Q4_MEASURE;
                    Q4[2] = (double)(tmpBuffer[8]<<8|tmpBuffer[9])/Q4_MEASURE;
                    Q4[3] = (double)(tmpBuffer[10]<<8|tmpBuffer[11])/Q4_MEASURE;
                    display_Q4decodeData(Q4);
                    imu_data.orientation.x = Q4[1];
                    imu_data.orientation.y = Q4[2];
                    imu_data.orientation.z = Q4[3];
                    imu_data.orientation.w = Q4[0];
                    
                    break;
                }


            case DATA_TYPE_GRY: //该帧输出数据为陀螺仪数据类型
                {                
                    GYR[0] = (double)(tmpBuffer[4]<<8|tmpBuffer[5]);
                    GYR[1] = (double)(tmpBuffer[6]<<8|tmpBuffer[7]);
                    GYR[2] = (double)(tmpBuffer[8]<<8|tmpBuffer[9]);
                    display_decodeData(ACC);
                    imu_data.angular_velocity.x = GYR[0];
                    imu_data.angular_velocity.y = GYR[1];
                    imu_data.angular_velocity.z = GYR[2];
                    
                    break;
                }

            case DATA_TYPE_ACC:  //该帧输出数据为加速度类型
                 { 
                     ACC[0] = (double)(tmpBuffer[4]<<8|tmpBuffer[5]);
                     ACC[1] = (double)(tmpBuffer[6]<<8|tmpBuffer[7]);
                     ACC[2] = (double)(tmpBuffer[8]<<8|tmpBuffer[9]);
                     display_decodeData(ACC);
                     imu_data.linear_acceleration.x = ACC[0];
                     imu_data.linear_acceleration.y = ACC[1];
                     imu_data.linear_acceleration.z = ACC[2];
                     
                     break;
                 } 

            case DATA_TYPE_MAG:  {std::cout<<"未更新DATA_TYPE_MAG解码程序！"<<std::endl;break;}   //该帧输出数据为磁力计类型

            case DATA_TYPE_EULAR:  //解码欧拉角原始数据
                { 
                     EULAR[0] = (double)(tmpBuffer[4]<<8|tmpBuffer[5])/EULAR_MEASURE;
                     EULAR[1] = (double)(tmpBuffer[6]<<8|tmpBuffer[7])/EULAR_MEASURE;
                     EULAR[2] = (double)(tmpBuffer[8]<<8|tmpBuffer[9])/EULAR_MEASURE;
                     display_decodeData(EULAR);
                     break;
                }

            case DATA_TYPE_STOP: {std::cout<<"未更新DATA_TYPE_STOP解码程序！"<<std::endl;break;} //该帧输出数据为保留不用类型

            case DATA_TYPE_PRECISION:{std::cout<<"未更新DATA_TYPE_PRECISION解码程序！"<<std::endl;break;}  //该帧输出数据为传感器精度，频率类型

            case DATA_TYPE_RANGE: {std::cout<<"未更新DATA_TYPE_RANGE解码程序！"<<std::endl;break;}  //该帧输出数据为传感器量程
            default: break;
        }

    }
    else
    {
        std::cout<<"数据帧头错误"<<std::endl;
        return false;
    }
    return true;
}


bool IPSG::CImuCommand::cmdFrame(unsigned char imucmd)
{
    cmd_buffer[0] = CMD_FrameHead;
    cmd_buffer[1] = imucmd;
    cmd_buffer[2] = CMD_FrameHead + cmd_buffer[1];
    //return commd_buffer;

    ser.write(cmd_buffer,cmd_num);
    
    ser.read(r_buffer,READ_BUFFERSIZE);
    
    for(int i=0;i<READ_BUFFERSIZE;i++)
    {
        //read_buffer[i] = (float)r_buffer[i];
        //ROS_INFO("[0x%02x]",r_buffer[i]);
    }
    //ROS_INFO("DECODE OUTPUT DATA!");
    decodeFrame(r_buffer);
    return true;
}


bool IPSG::CImuCommand::muliteCmdFrame(unsigned char imucmd1,unsigned char imucmd2,unsigned char imucmd3)
{
    //第一个指令
    cmdFrame(imucmd1);
    ROS_INFO("OUTPUT_Q4");   //如何输出宏定义的名字？？？？

    //第二个指令
    cmdFrame(imucmd2);
    ROS_INFO("OUTPUT_GYR");

    //第三个指令
    cmdFrame(imucmd3);
    ROS_INFO("OUTPUT_ACC");

return true;
}


bool IPSG::CImuCommand::serialInit()
{
        try
    {
        ser.setPort("/dev/ttyUSB0");
        ser.setBaudrate(115200);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        ser.setTimeout(to);
        ser.open();
    }
    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port ");
        return -1;
    }

    if(ser.isOpen()){
        ROS_INFO_STREAM("Serial Port initialized");
        cmdFrame(CMD_OUTPUT_200HZ);
        ser.write(cmd_buffer,cmd_num);
    }else{
        return -1;
    }
    return true;
}


bool IPSG::CImuCommand::RUN()
{   
    
    ros::NodeHandle nh;

   // ros::Subscriber write_sub = nh.subscribe("write", 1000, write_callback);
    ros::Publisher msg_pub = nh.advertise<sensor_msgs::Imu>("imumsg", 1000);

    //串口初始化
    serialInit();
    ros::Rate loop_rate(50);
    while(ros::ok())
    {
        //serial_msgs::serial msg;
        
        //ros::spinOnce();
        //ROS_INFO_STREAM("0000");
        // if(ser.available())
        //{
            //ROS_INFO_STREAM("11111");
            muliteCmdFrame(CMD_PULL_OUTPUT_Q4,CMD_PULL_OUTPUT_GYR,CMD_PULL_OUTPUT_ACC);
            display_Imumsg(imu_data);
            msg_pub.publish(imu_data);
            /*
            ROS_INFO("Reading from serial port");
            ser.read(r_buffer,rBUFFERSIZE);
            for(int i=0;i<rBUFFERSIZE;i++)
            {
                ROS_INFO("[0x%02x]",r_buffer[i]);
            }
            ROS_INFO_STREAM("End reading from serial port");

                for(int l=0;l<rBUFFERSIZE;l++)
                {
                    msg.serial.push_back(r_buffer[l]);
                    msg_pub.publish(msg);*/
        /*  for(int j=0;j<rBUFFERSIZE;j++)
         * ROS_INFO("[0x%02x]",msg.serial[j]);*/
        //}
            loop_rate.sleep();
    }

        return true;
}
