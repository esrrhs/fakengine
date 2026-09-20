package main

import (
	"encoding/xml"
	"fmt"
	"os"
	"strconv"
	"text/template"

	"golang.org/x/text/encoding/simplifiedchinese"
)

type Member struct {
	Name    string `xml:"name,attr"`
	Comment string `xml:"comment,attr"`
	Cmp     string `xml:"cmp,attr"`
}

type Array struct {
	Name    string `xml:"name,attr"`
	Comment string `xml:"comment,attr"`
	Length  string `xml:"length,attr"`
}

type TopArray struct {
	Name    string `xml:"name,attr"`
	Comment string `xml:"comment,attr"`
	Length  string `xml:"length,attr"`
	Top     string `xml:"top,attr"`
	Cmp     string `xml:"cmp,attr"`
}

type HashMap struct {
	Name    string `xml:"name,attr"`
	Comment string `xml:"comment,attr"`
	Length  string `xml:"length,attr"`
	Top     string `xml:"top,attr"`
	Cmp     string `xml:"cmp,attr"`
}

type Struct struct {
	Name      string     `xml:"name,attr"`
	Comment   string     `xml:"comment,attr"`
	Members   []Member   `xml:"member"`
	Arrays    []Array    `xml:"array"`
	TopArrays []TopArray `xml:"toparray"`
	HashMaps  []HashMap  `xml:"hashmap"`
}

type Result struct {
	XMLName xml.Name `xml:"result"`
	Structs []Struct `xml:"struct"`
}

var result = Result{}

func main() {
	if !parse() {
		return
	}
	if !output("StatMng_h.tpl", "StatMng.h") {
		return
	}
	fmt.Println("OK")
}

func parse() bool {
	data, err := os.ReadFile("stat.xml")
	if err != nil {
		fmt.Println(err)
		return false
	}
	if err := xml.Unmarshal(data, &result); err != nil {
		fmt.Println(err)
		return false
	}
	return true
}

func genlist(n string) []string {
	num, _ := strconv.Atoi(n)
	ret := make([]string, num)
	for i := 0; i < num; i++ {
		ret[i] = strconv.Itoa(i)
	}
	return ret
}

// iconv encodes template comments to GBK (legacy header encoding).
// GBK is a superset of the old GB2312 path that mahonia used.
func iconv(str string) string {
	out, err := simplifiedchinese.GBK.NewEncoder().String(str)
	if err != nil {
		return str
	}
	return out
}

func output(src, des string) bool {
	tpl, err := os.ReadFile(src)
	if err != nil {
		fmt.Println(err)
		return false
	}

	t, err := template.New(src).Funcs(template.FuncMap{
		"genlist": genlist,
		"iconv":   iconv,
	}).Parse(string(tpl))
	if err != nil {
		fmt.Println(err)
		return false
	}

	file, err := os.Create(des)
	if err != nil {
		fmt.Println(err)
		return false
	}
	defer file.Close()

	if err := t.Execute(file, result.Structs); err != nil {
		fmt.Println(err)
		return false
	}
	return true
}
