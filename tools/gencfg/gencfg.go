package main

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"strings"
	"text/template"
)

type Member struct {
	Name    string
	Comment string
	Default string
}

type Struct struct {
	Name    string
	Comment string
	Members []Member
}

type Result struct {
	SrcName string
	Name    string
	Structs []Struct
}

var result = Result{}

func main() {
	if len(os.Args) < 3 {
		fmt.Println("usage: gencfg SrcFileName DstName")
		return
	}

	srcFile := os.Args[1]
	desFile := os.Args[2]

	fmt.Println("Read " + srcFile)
	fmt.Println("Gen " + desFile + "CfgLoader.h")

	result.SrcName = srcFile
	result.Name = desFile

	if !parse(srcFile) {
		return
	}
	if !output("CfgLoader_h.tpl", desFile+"CfgLoader.h") {
		return
	}
	fmt.Println("OK")
}

func parse(filename string) bool {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Println(err)
		return false
	}
	defer file.Close()

	rd := bufio.NewReader(file)
	comment := ""

	for {
		line, err := rd.ReadString('\n')
		if err != nil {
			if err != io.EOF {
				fmt.Println(err)
				return false
			}
			if line == "" {
				break
			}
		}

		line = strings.Trim(line, "\n")
		line = strings.Trim(line, "\r")
		line = strings.TrimLeft(line, " ")
		line = strings.TrimRight(line, ";")
		line = strings.TrimRight(line, "#")
		if len(line) == 0 {
			if err == io.EOF {
				break
			}
			continue
		}

		if line[0] == '#' {
			comment = strings.TrimLeft(line, "#")
		} else if line[0] == '[' {
			end := strings.Index(line, "]")
			if end == -1 {
				fmt.Println("no ] in " + line)
				return false
			}
			name := line[1:end]
			result.Structs = append(result.Structs, Struct{Name: name, Comment: comment})
		} else {
			end := strings.Index(line, "=")
			if end == -1 {
				fmt.Println("no = in " + line)
				return false
			}
			name := strings.Trim(line[0:end], " ")
			defaultval := strings.Trim(line[end+1:], " ")
			m := Member{Name: name, Comment: comment, Default: defaultval}
			result.Structs[len(result.Structs)-1].Members = append(
				result.Structs[len(result.Structs)-1].Members, m)
		}

		if err == io.EOF {
			break
		}
	}
	return true
}

func output(src, des string) bool {
	tpl, err := os.ReadFile(src)
	if err != nil {
		fmt.Println(err)
		return false
	}

	t, err := template.New(src).Parse(string(tpl))
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

	if err := t.Execute(file, result); err != nil {
		fmt.Println(err)
		return false
	}
	return true
}
