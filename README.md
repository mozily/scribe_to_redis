# scribe_to_redis

![N|Solid](https://a.fsdn.com/con/app/proj/scribeserver/screenshots/193238.jpg)

# how to install
#### 1. requirement packages install
```
sudo yum -y install redis
sudo yum -y install automake libtool flex bison pkgconfig gcc-c++ boost-devel libevent-devel zlib-devel python-devel ruby-devel openssl-devel wget make tar git
```
#### 2. thrift & fb303 install
```
cd /tmp
wget https://github.com/mozily/scribe_to_redis.git

tar -xzvf /tmp/scribe_to_redis/thrift/thrift-0.9.0.tar.gz
cd /tmp/scribe_to_redis/thrift-0.9.0
./configure
make
make install
find /usr/local -name *thrift*

cd /tmp/scribe_to_redis/thrift-0.9.0/contrib/fb303
./bootstrap.sh
./configure CPPFLAGS="-DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H"
make
make install
find /usr/local -name *fb303*
```
#### 3. hiredis install
```
cd /tmp/scribe_to_redis/hiredis
make
make install
ldconfig
```
#### 4. scribe install
```
cd /tmp/scribe_to_redis/scribe
chmod 555 bootstrap.sh
./bootstrap.sh
./configure --with-boost-system=boost_system-mt --with-boost-filesystem=boost_filesystem-mt CPPFLAGS="-DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H"
make
make install
find /usr/local -name *scribe*
```
#### 5. scribe setting
```
cd /tmp/scribe_to_redis/scribe/examples
echo "/usr/local/lib" > /etc/ld.so.conf.d/scribed.conf
ldconfig
chmod 755 scribe_cat
cp -f scribe_cat /usr/local/bin
chmod 755 scribe_ctrl
cp -f scribe_ctrl /usr/local/bin
mkdir /etc/scribed
cp redis_example.conf /etc/scribed/scribed.conf
echo "SCRIBED_CONFIG=/etc/scribed/scribed.conf" >> /etc/sysconfig/scribed
cat scribed > /etc/init.d/scribed
chmod ugo+x /etc/init.d/scribed
chkconfig scribed on
/etc/init.d/scribed start
```
# how to use
#### 1. run redis
```
/etc/init.d/redis start
telnet redis-host redis-port
ex) telnet localhost 6379
```
#### 2. scribe configure check
```
vi /etc/scribed/scribed.conf
```
#### 3. scribe data send
```
echo "scribe data send test" | /usr/local/bin/scribe_cat test
```
#### 4. redis data check
```
telnet redis-host redis-port
rpop test
subscribe test
```
