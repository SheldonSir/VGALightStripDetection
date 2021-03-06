工作备忘：
1. check 程序支持的最低软硬件环境+Camera性能
2. 在确定置具和平台位置后，ROI区域可以考虑一个定值，以避免操作员误操作
3.. 在程序通过按ESC键退出时，不关机
4.. 捕捉程序异常， 部分USB口松动会导致相机抓不到图，程序闪退
5. 相机曝光设置为"自动"，显卡断电启动时，发出默认光，会干扰相机曝光设定，造成过曝问题
    将目前测试相机曝光设定为[-2, -3]，效果明显好转
    故正式线上相机要能设定此属性
6. 目前有两种阈值需要设定，一种是基于BGR的阈值设定，另一种是基于HSV的阈值设定。先通过BGR阈值将大部分像素点过滤，然后在对mask进行一次HSV阈值设定。
但目前就HSV设定的阈值范围来看，范围很大，几乎可以忽略HSV设定
  另外，目前在不同位置的平台，需要手动调节阈值范围，需要考虑更加灵活的方式，来自动调节
7. 模板匹配可以考虑进来，模板轮廓的制作，匹配的置信度 -- 此路不通


Version Log
[v2.0.0.1]
1. 基于帧差法的大框架下，frame - background 后的mask 通过自适应阈值来调整掩膜
2. 再对mask覆盖的区域轮廓进行逐个颜色校验


[v2.0.0.2]
0. 针对取图逻辑有概率出现连续抢到CPU时间片， 导致无法更新到最新照片问题， 将取图逻辑设置为直接从相机中拿图，跟拍摄线程不打架，这样看拍摄线程的意义有待商榷
1. 增加饱和度设置，通过将饱和度拉到最大128， 来提升获取到的颜色鲜艳度
2. 查不到PPID时， 生成一个GUID来替换，并将涉计到的路径buf长度提高到MAX_PATH
3. 优化选取ROI逻辑

[v2.0.0.3]
1. 接入Spdlog日志库
2. 解掉复测时无法保存绿色和红色的图片的bug

[v2.0.0.4]
1. 正式程序里只测1次，如果需要测试N次，用批处理进行调用
2. (g_Config.shutdownTime >= ePowerOff && g_main_thread_exit == eExit) 这里的退出逻辑需要调整，有点问题
    这部分现在调整为 只认 g_Config.shutdownTime 状态信息
3. PPID 传参给main - 暂时未接
4. 程序中添加ErrorCode，通过抛出异常来处理
5. 自动获取灯带整体轮廓区域
6. 开始测试后，键入 0 表示本次测试完不关机

[v2.0.0.5] - 2021.04.13
1. 從MCU中抓取LED燈數 --> 已接入
2. PPID，显卡料号通过bat抓取并以參數形式傳入，程式中不再抓取
3. I2C報異常時，aging.csv數據保存為Fail
4. spdlog 直接保存到目的文件夹中，不再分散保存
5. 启动AP即开启工作线程，直到相机初始化完成，工作线程才正式工作
6. 22颗灯的情况下，点亮22颗灯需要300ms - 350ms 左右， 点亮单颗灯平均15ms左右
7. 调整3c.ini配置档结构，删除无用部分
8. 自動抓取燈帶ROI 兼容TUF卡

