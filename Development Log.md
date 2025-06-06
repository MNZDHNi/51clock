# Development Log
## 6/5
- [x] 解决git推送拉取出错的问题
- [x] 修改重复时间设置的bug

---

## 6/6
- [ ] 完成蜂鸣器的部分
- [ ] 调试中断和计时器(T0/T1/T2)
- [ ] 添加当前模式显示

---

考虑AlarmSet 是否需要定时器  
将 AlarmSet 添加 减 功能，增加边界判断，`clock[] >= DS1302_Time[]` ？  
使用 DS1302 时间完成 WatchClock ?  
使用中断功能 完成

---
### Alarmset
添加蜂鸣器触发条件判断 写入 `while(1)` 中，与 `KeyNum` 共同判断

---
### WatchClock
通过定时器累计时间  
新建 结构体数组 存储 回看的时间
