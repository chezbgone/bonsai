''' author: samtenka
    change: 2020-05-23
    create: 2020-05-22
    descrp: 
    to use: 
'''

import glob
import numpy as np

def parse(file_nm): 
    with open(file_nm) as f:
        lines = f.read().split('\n')
    
    types = [] 
    includes = []
    parsed = [['supersection', file_nm.replace('_', '\\_'), 0]]
    
    l = 0 
    while l != len(lines):  
        if lines[l].strip().startswith('typedef'): 
            types.append(lines[l].split(';')[0].strip().split()[-1]) 
        if lines[l].startswith('#include "'): 
            includes.append(lines[l].split()[1].split('/')[-1].replace('"', ''))

        if lines[l].strip().startswith('/*='): 
            title = ' '.join(lines[l+1].strip().split(' ')[2:-1])
            indent = 0 #len(lines[l+1])-len(lines[l+1].lstrip()) 
            parsed.append(['section', title, indent])
            l += 3; continue
        elif lines[l].strip().startswith('/*~'): 
            title = ' '.join(lines[l+1].strip().split(' ')[2:-1])
            indent = len(lines[l+1])-len(lines[l+1].lstrip()) 
            parsed.append(['subsection', title, indent])
            l += 2; continue
        elif lines[l].strip().startswith('/*-'): 
            title = ' '.join(lines[l+0].strip().split(' ')[2:-1])
            indent = len(lines[l+0])-len(lines[l+0].lstrip()) 
            parsed.append(['subsubsection', title, indent])
        elif lines[l].strip():
            if not parsed or parsed[-1][0] != 'body': 
                parsed.append(['body', '', None])
            parsed[-1][1] += lines[l]+'\n'
        l += 1
            
    return types, includes, parsed

def texify(tag, data, indent):
    if tag == 'body':
        return (
            #'{\\footnotesize\\begin{lstlisting}\n'
            '\n\n\\footnotesize\\begin{minted}{C}\n'
            + data +  
            '\n\\end{minted}\n\n'
            #'\\end{lstlisting}}\n'
        )
    else:
        return {
            'supersection': '\\moosupersection{{{}}}{{{}}}',
            'section': '\\moosection{{{}}}{{{}}}',
            'subsection': '\\moosubsection{{{}}}{{{}}}',
            'subsubsection': '\\moosubsubsection{{{}}}{{{}}}',
        }[tag].format('m'*indent, data)

def normalize_name(file_nm):
    kk = file_nm.split('.')[0]
    return kk.split('_')[0] if not kk.startswith('test') else kk

def make_dep_table(dep_dict):
    new_dep_dict = {}
    for k in dep_dict:
        kk = normalize_name(k)
        if kk not in new_dep_dict:
            new_dep_dict[kk] = set()
        new_dep_dict[kk].update( map(normalize_name, dep_dict[k]) )
    dep_dict = new_dep_dict

    ll = list(dep_dict.keys())
    mm = np.array([
        [
            1.0 if ll[i] in dep_dict[ll[j]] else 0.0
            for j in range(len(ll))
        ]
        for i in range(len(ll))
    ]) + 10.0 * np.eye(len(ll))
    mm3 = np.matmul(mm, np.matmul(mm, mm))
    w, v = np.linalg.eig(mm)
    ii = max((w[i], i) for i in range(len(w)))[1]
    v = [vv[ii] for vv in v]

    H, W = 14, 9
    fnm_by_pos = {} 
    pos_by_fnm = {} 

    min_r = min(np.sum(mm3[:,i]) for i in range(len(ll)))
    max_r = max(np.sum(mm3[:,i]) for i in range(len(ll)))
    for nm in dep_dict: 
        i = ll.index(nm)
        r = np.random.binomial(H-1, p = max(0.1, min(0.9, (np.sum(mm3[:,i])-min_r)/(max_r-min_r))) ) 
        c = np.random.binomial(W-1, p = max(0.1, min(0.9, (v[i]-min(v))/(max(v)-min(v)))) ) 
        while (r, c) in fnm_by_pos: 
            r = np.random.binomial(H-1, p = max(0.1, min(0.9, (np.sum(mm3[:,i])-min_r)/(max_r-min_r))) ) 
            c = np.random.binomial(W-1, p = max(0.1, min(0.9, (v[i]-min(v))/(max(v)-min(v)))) ) 
        fnm_by_pos[(r,c)] = nm
        pos_by_fnm[nm] = (r,c)

    body = ''
    for r in range(H): 
        for c in range(W): 
            if (r,c) in fnm_by_pos:
                body += '{{ {} {} }}'.format(
                    '\\color{mylime}' if fnm_by_pos[(r,c)].startswith('test_') else '\\color{mypurp}',
                    fnm_by_pos[(r,c)].replace('_', '\\_')
                )
                for incl in dep_dict[fnm_by_pos[(r,c)]]:
                    rr, cc = pos_by_fnm[incl] 
                    if (rr,cc)==(r,c): continue
                    body += ' \\arrow[mygray, {}, bend right=10]'.format(
                        'u'*(r-rr)+
                        'd'*(rr-r)+
                        'l'*(c-cc)+
                        'r'*(cc-c)
                    )
            else:
                body += '{}'
            if c+1 != W: body += ' & '
        if r+1 != H: body += ' \\\\ \n'
    return body

def do_all():
    DEPS = {}
    TYPES = [] 
    CODE = '' 
    for file_nm in sorted(glob.glob('../[!v]*/*.[c|h]')):
        types, includes, parsed = parse(file_nm) 
        DEPS[file_nm.split('/')[-1]] = set(includes)
        CODE += '\n\n'.join(texify(tag, data, indent) for tag, data, indent in parsed)
        print(file_nm, includes)
        TYPES += types
    print('FOUND TYPES:', ', '.join(TYPES))
    print('FOUND DEPS:', DEPS)

    with open('template.tex') as f:
        tex = f.read()
    
    with open('out.tex', 'w') as f:
        f.write(tex
            .replace('CODE', CODE)
            .replace('DEP_TABLE', make_dep_table(DEPS))
            .replace('TYPES', ','.join(TYPES))
        )

if __name__=='__main__':
    do_all()
