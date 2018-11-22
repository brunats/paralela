package main


import (
	"fmt"
//	"math/big"
    "time"
)

func ehPrimo(num int64) bool {
    var i int64 = 2
    for i < num {
        if(num % i == 0){
            return false
        }
        i+= int64(1)
    }
    return true
}

func main() {
    var n int64 = int64(998300000000000000)
    start := time.Now()
    res := ehPrimo(n)
    t := time.Now()
    elapsed := t.Sub(start)
    fmt.Println(res, elapsed)
}
