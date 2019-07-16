## Omocha OS - A Personal OS Project For Study

# How to Build

1. Create a disk image with a dos partition table type and a fat32 part inside it.
```
dd if=/dev/zero of=HD.img bs=1024 count=81920
sudo cfdisk HD.img # and choose dos for it, then create a fat32 part.
sudo losetup -fP HD.img
sudo losetup -a | grep "HD.img"
sudo mkfs.fat -F 32 /dev/loopXp1 # X is acquired above
```
2. Place disk image inside `src/boot/tools/` and filename should be `HD.img`
3. `make && make`
4. make boot

# What am I thinking

编写一个操作系统从初中开来就是我的梦想，花花绿绿的界面、动画流畅的UI，向来对我的吸引力没有黑框框白点点的命令行强大。

而令人最向往的是，埋藏在这些命令行背后的操盘手——操作系统。

年少无知，曾多次尝试过踏入次领域，但是终究未果。

然向往之心却若墨之入水，渐衍渐深。

在2018年的暑假，我终于开始重新拾起若干年前买的书

在参加MakeX集训的时候，通读了大半本。

愈读愈想自己动手书写，却因能力不足而渐渐开始从书中的示例代码中复制，最后因为自己写的部分和示例无法切合，让自己写的代码被复制之恶习蚕食殆尽。

而此项目，则是我励志革新之做

不敢说成功，不敢说优秀

但求问心无愧

不敢说不抄，不敢说原创

但求行行求真

凡是写入这个仓库的代码，必定是我了然于心

尽然Bug天意，本人愚笨，尚且不能完全弄清，但日日解决日日翻新，便也是循环向上之意。

初为32位，后定能成为64位，这是我的目标，也是我高中三年的愿望。

这个项目吸收了很多来自Orange和xv6的源码，对先辈们的贡献深表感谢。

———— Shiroko，一个爱好计算机科学的懵懂中学生
