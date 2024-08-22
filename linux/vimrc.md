## vimrc 설정
<br>
# 1. 행 번호 출력
set nu
<br>
# 2. tab 자동완성

```
function InsertTabWrapper()
    let col = col('.') - 1
    if !col || getline('.')[col - 1] !~ '\k'
        return "\<tab>"
    else
        return "\<c-n>"
    endif
endfunction
 
inoremap <tab> <c-r>=InsertTabWrapper()<cr>
```

<br>
# 3. 자동 들여쓰기
set cindent
