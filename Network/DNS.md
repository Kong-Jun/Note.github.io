Berkeley Internet Name Domain (BIND) software UDP port 53
DNS 的作用
DNS 的架构
DNS 查询的过程
    recursive/iterative queries
    DNS caching -- local DNS
        local DNS会告知是否来自 cache(nslookup: non-authoritive anwser)
        default DNS
DNS 消息（不重要）
    DNS消息中可以有多个 RR，比如authoritive dns 的名字和地址（两条 RR）
DNS 更新
    UPDATE 动态更新
nslookup
安全问题
    中间人攻击，DDoS
    DNS 安全性良好，目前没有出现过大规模的安全事故
配置 DNS
    127.0.0.53 和 loopback network interface???    https://unix.stackexchange.com/questions/612416/why-does-etc-resolv-conf-point-at-127-0-0-53
