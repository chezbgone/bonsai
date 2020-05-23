import glob
file_nm = '../lambda/extract.c' 

def parse(file_nm): 
    with open(file_nm) as f:
        lines = f.read().split('\n')
    
    types = [] 
    parsed = [['supersection', file_nm.replace('_', '\\_')]]
    
    l = 0 
    while l != len(lines):  
        if lines[l].strip().startswith('typedef'): 
            types.append(lines[l].split(';')[0].strip().split()[-1]) 
            print(file_nm, lines[l])

        if lines[l].strip().startswith('/*='): 
            title = ' '.join(lines[l+1].strip().split(' ')[2:-1])
            parsed.append(['section', title])
            l += 3; continue
        elif lines[l].strip().startswith('/*~'): 
            title = ' '.join(lines[l+1].strip().split(' ')[2:-1])
            parsed.append(['subsection', title])
            l += 2; continue
        elif lines[l].strip().startswith('/*-'): 
            title = ' '.join(lines[l+0].strip().split(' ')[2:-1])
            parsed.append(['subsection', title])
        elif lines[l].strip():
            if not parsed or parsed[-1][0] != 'body': 
                parsed.append(['body', ''])
            parsed[-1][1] += lines[l]+'\n'
        l += 1
            
    return types, parsed

def texify(tag, data):
    if tag == 'body':
        return (
            '{\\footnotesize\\begin{lstlisting}[language=C]\n'
            + data +  
            '\\end{lstlisting}}\n'
        )
    else:
        return {
            'supersection': '\\moosupersection{{ {} }}',
            'section': '\\moosection{{ {} }}',
            'subsection': '\\moosubsection{{ {} }}',
            'subsubsection': '\\moosubsubsection{{ {} }}',
        }[tag].format(data)

def do_all():
    TYPES = [] 
    CODE = '' 
    for file_nm in sorted(glob.glob('../[!v]*/*.[c|h]')):
        types, parsed = parse(file_nm) 
        CODE += '\n\n'.join(texify(tag, data) for tag, data in parsed)
        TYPES += types
    print(TYPES)

    with open('template.tex') as f:
        tex = f.read()
    
    with open('out.tex', 'w') as f:
        f.write(tex.replace('CODE', CODE).replace('TYPES', ','.join(TYPES)))

if __name__=='__main__':
    do_all()
