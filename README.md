# shamir-algorithm_with-Large-number-operation
有时候，秘密不能由一个人独自掌握，而需要多人参与才能打开秘密，比如说核导弹发射密码，
我们不想让一个人独自握有发射密码以防泄露等特殊情况，为此我们需要把密码分给多人，每个
人握有用于恢复密码的部分信息，握有子秘钥的一定人数的人可以恢复密码发射核弹。这种技术
被称为秘密分割(secret splitting)或秘密分享(secret sharing)。
假设存在一个秘密s，现在需要把秘密分给n个用户P1, ..., Pn，每个人获得一部分信息si，这部分
信息被称为一个子秘钥或影子(share or shadow)，需要至少t个用户提供他们所拥有的子秘钥才能
恢复出秘密s，这样一种方案被称为(t,n)门限秘密共享方案。如果任意t-1个用户提供他们的子秘
钥都无法获得关于s的任何信息，就称该方案是完善的。

由于项数较多会生成大数，引用了一个C++开源的高精度浮点数运算库。
![Alt text](https://github.com/12345-yjy/shamir-algorithm_with-Large-number-operation/blob/main/ScreenShot.bmp)

