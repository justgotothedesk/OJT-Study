# 1. 특정 파일에서 원하는 줄만 출력하는 명령어는?
# seq 1 20 > numbers.txt
sed -n '10, 14p' numbers.txt

# 2. 특정 파일의 내용 중에 특정 키워드가 포함된 라인을 특정 파일에 저장하는 명령어는?
grep 'systemd' /var/log/messages > filter.txt

# 3. 하위 디렉토리에 있는 파일들 중에서 특정 확장자를 가진 모든 파일을 삭제하는 명령어는?
rm *.txt

# 4. 현재 실행 중인 모든 프로세스 중에서 특정 이름을 가진 프로세스의 PID, CPU, MEM 정보를 포함하여 검색하는 명령어는?
ps aux | grep 'target'

# 5. 특정 user가 실행 중인 프로세스를 실시간으로 사용 중인 특정 리소스 순으로 정렬하여 확인하는 방법은?
ps aux --sort=%mem # 오름차순
ps aux --sort=-%mem # 내림차순

# 1. 하위 디렉토리의 특정 depth까지의 디스크 사용량을 조회하고 용량 단위를 붙여 출력하는 명령어는?
du -h --max-depth=2 | sort -hr
du -h -b --max-depth=2 | sort -hr # byte 단위로 조회

# 2. 현재 디렉터리에 있는 파일/디렉터리들 중에서 이름에 특정 키워드가 들어간 파일/디렉토리의 갯수를 출력하는 명령어는?
ls | grep -c 'target'

# 3. 하위 디렉터리에 있는 파일들 중에서 파일명 중간의 특정 키워드가 들어가고 특정 확자자를 가진 파일을 모두 검색해서 특정 파일에 쓰는 명령어는?
find . -name '*target*' > filter.txt

# 4. 네트워크 인터페이스 별 RX/TX 정보를 특정 시간마다 갱신하여 차이점을 강조하여 보여주는 명령어는?
ifconfig 모르겠다 ㅠㅠ

# 5. 현재 디렉토리 하위에 있는 파일들 중에서 파일명에 특정 패턴을 포함하고 있으며, 본문에 대소문자 상관 없이 특정 키워드가 들어가는 파일들을 찾아 특정 확장자를 가진 파일을 제외한 나머지 결과를 지정한 파일에 쓰는 명령어는?
grep -iR --include="*filename*" --exclud="*.txt" "searchname" . > filter.txt

# 6. 원격 장치의 특정 디렉토리에 있는 실행 파일을 원격으로 실행하는 명령어는?
ssh user@remote_host '/path'

# 7. 특정 디렉토리에 있는 특정 확장자 파일에서 특정 문구를 모두 원하는 문구로 수정하는 명령어는?
find /etc/yum/repo.d/ -name "*.repo" -exec sh -c '
for file do
    cp "$file" "$file.back"
    sed -i "s/enabled=1/enabled=0/g" "$file"
done
' sh {} +

# 8. 현재 디렉터리에 있는 파일/디렉터리를 파일 크기 순으로 정렬하고 파일 크기가 큰 top 10개를 출력하기
ls -al | head -n 10

# 9-1. 이전에 수행한 명령어들의 리스트 번호를 이용하여 명령어 수행하기
history

# 9-2. 이전에 수행한 명령어를 재입력하지말고 리스트 번호를 이용하여 수행
!리스트번호

# 10. 부모 디렉터리까지 한번에 디렉터리 만들기
mkdir -p /secudaim/test1/db/test2

# 11. 다른 장비에 파일 복사하기
흠 모르겠네