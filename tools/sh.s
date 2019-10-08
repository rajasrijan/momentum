
sh.elf:     file format elf64-x86-64


Disassembly of section .text:

00000000004000f0 <main>:
 * along with Momentum.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>

int main()
{
  4000f0:	55                   	push   rbp
  4000f1:	48 89 e5             	mov    rbp,rsp
  4000f4:	48 81 ec 20 10 00 00 	sub    rsp,0x1020
  4000fb:	31 f6                	xor    esi,esi
  4000fd:	b8 00 10 00 00       	mov    eax,0x1000
  400102:	89 c2                	mov    edx,eax
  400104:	c7 45 fc 00 00 00 00 	mov    DWORD PTR [rbp-0x4],0x0
    char cmdline[4096] = {0};
  40010b:	48 8d 8d f0 ef ff ff 	lea    rcx,[rbp-0x1010]
  400112:	48 89 cf             	mov    rdi,rcx
  400115:	e8 36 00 00 00       	call   400150 <memset>
    while (1)
    {
        printf("$");
  40011a:	48 bf 40 26 40 00 00 	movabs rdi,0x402640
  400121:	00 00 00 
  400124:	b0 00                	mov    al,0x0
  400126:	e8 65 0a 00 00       	call   400b90 <printf>
  40012b:	b9 00 10 00 00       	mov    ecx,0x1000
  400130:	89 ce                	mov    esi,ecx
  400132:	48 8d bd f0 ef ff ff 	lea    rdi,[rbp-0x1010]
        gets_s(cmdline, sizeof(cmdline));
  400139:	89 85 ec ef ff ff    	mov    DWORD PTR [rbp-0x1014],eax
  40013f:	e8 5c 11 00 00       	call   4012a0 <gets_s>
    while (1)
  400144:	48 89 85 e0 ef ff ff 	mov    QWORD PTR [rbp-0x1020],rax
  40014b:	e9 ca ff ff ff       	jmp    40011a <main+0x2a>

0000000000400150 <memset>:
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

void *memset(void *ptr, int value, size_t num)
{
  400150:	55                   	push   rbp
  400151:	48 89 e5             	mov    rbp,rsp
  400154:	48 89 7d f8          	mov    QWORD PTR [rbp-0x8],rdi
  400158:	89 75 f4             	mov    DWORD PTR [rbp-0xc],esi
  40015b:	48 89 55 e8          	mov    QWORD PTR [rbp-0x18],rdx
    for (size_t i = 0; i < num; i++)
  40015f:	48 c7 45 e0 00 00 00 	mov    QWORD PTR [rbp-0x20],0x0
  400166:	00 
  400167:	48 8b 45 e0          	mov    rax,QWORD PTR [rbp-0x20]
  40016b:	48 3b 45 e8          	cmp    rax,QWORD PTR [rbp-0x18]
  40016f:	0f 83 21 00 00 00    	jae    400196 <memset+0x46>
    {
        ((char *)ptr)[i] = (char)(value);
  400175:	8b 45 f4             	mov    eax,DWORD PTR [rbp-0xc]
  400178:	88 c1                	mov    cl,al
  40017a:	48 8b 55 f8          	mov    rdx,QWORD PTR [rbp-0x8]
  40017e:	48 8b 75 e0          	mov    rsi,QWORD PTR [rbp-0x20]
  400182:	88 0c 32             	mov    BYTE PTR [rdx+rsi*1],cl
    for (size_t i = 0; i < num; i++)
  400185:	48 8b 45 e0          	mov    rax,QWORD PTR [rbp-0x20]
  400189:	48 83 c0 01          	add    rax,0x1
  40018d:	48 89 45 e0          	mov    QWORD PTR [rbp-0x20],rax
  400191:	e9 d1 ff ff ff       	jmp    400167 <memset+0x17>
    }
    return ptr;
  400196:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  40019a:	5d                   	pop    rbp
  40019b:	c3                   	ret    
  40019c:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]

00000000004001a0 <memcpy>:
}

void *memcpy(void *dst, const void *src, size_t len)
{
  4001a0:	55                   	push   rbp
  4001a1:	48 89 e5             	mov    rbp,rsp
  4001a4:	48 89 7d f8          	mov    QWORD PTR [rbp-0x8],rdi
  4001a8:	48 89 75 f0          	mov    QWORD PTR [rbp-0x10],rsi
  4001ac:	48 89 55 e8          	mov    QWORD PTR [rbp-0x18],rdx
    for (size_t i = 0; i < len; i++)
  4001b0:	48 c7 45 e0 00 00 00 	mov    QWORD PTR [rbp-0x20],0x0
  4001b7:	00 
  4001b8:	48 8b 45 e0          	mov    rax,QWORD PTR [rbp-0x20]
  4001bc:	48 3b 45 e8          	cmp    rax,QWORD PTR [rbp-0x18]
  4001c0:	0f 83 27 00 00 00    	jae    4001ed <memcpy+0x4d>
    {
        ((char *)dst)[i] = ((char *)src)[i];
  4001c6:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  4001ca:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  4001ce:	8a 14 08             	mov    dl,BYTE PTR [rax+rcx*1]
  4001d1:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4001d5:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  4001d9:	88 14 08             	mov    BYTE PTR [rax+rcx*1],dl
    for (size_t i = 0; i < len; i++)
  4001dc:	48 8b 45 e0          	mov    rax,QWORD PTR [rbp-0x20]
  4001e0:	48 83 c0 01          	add    rax,0x1
  4001e4:	48 89 45 e0          	mov    QWORD PTR [rbp-0x20],rax
  4001e8:	e9 cb ff ff ff       	jmp    4001b8 <memcpy+0x18>
    }
    return dst;
  4001ed:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4001f1:	5d                   	pop    rbp
  4001f2:	c3                   	ret    
  4001f3:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
  4001fa:	00 00 00 
  4001fd:	0f 1f 00             	nop    DWORD PTR [rax]

0000000000400200 <strdup>:
}

char *strdup(const char *s1)
{
  400200:	55                   	push   rbp
  400201:	48 89 e5             	mov    rbp,rsp
  400204:	48 83 ec 20          	sub    rsp,0x20
  400208:	48 89 7d f8          	mov    QWORD PTR [rbp-0x8],rdi
    char *ret = (char *)malloc(strlen(s1) + 1);
  40020c:	48 8b 7d f8          	mov    rdi,QWORD PTR [rbp-0x8]
  400210:	e8 4b 00 00 00       	call   400260 <strlen>
  400215:	48 83 c0 01          	add    rax,0x1
  400219:	48 89 c7             	mov    rdi,rax
  40021c:	e8 9f 22 00 00       	call   4024c0 <malloc>
  400221:	48 89 45 f0          	mov    QWORD PTR [rbp-0x10],rax
    memcpy(ret, s1, strlen(s1) + 1);
  400225:	48 8b 7d f0          	mov    rdi,QWORD PTR [rbp-0x10]
  400229:	48 8b 75 f8          	mov    rsi,QWORD PTR [rbp-0x8]
  40022d:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  400231:	48 89 7d e8          	mov    QWORD PTR [rbp-0x18],rdi
  400235:	48 89 c7             	mov    rdi,rax
  400238:	48 89 75 e0          	mov    QWORD PTR [rbp-0x20],rsi
  40023c:	e8 1f 00 00 00       	call   400260 <strlen>
  400241:	48 83 c0 01          	add    rax,0x1
  400245:	48 8b 7d e8          	mov    rdi,QWORD PTR [rbp-0x18]
  400249:	48 8b 75 e0          	mov    rsi,QWORD PTR [rbp-0x20]
  40024d:	48 89 c2             	mov    rdx,rax
  400250:	e8 4b ff ff ff       	call   4001a0 <memcpy>
    return ret;
  400255:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  400259:	48 83 c4 20          	add    rsp,0x20
  40025d:	5d                   	pop    rbp
  40025e:	c3                   	ret    
  40025f:	90                   	nop

0000000000400260 <strlen>:
}

size_t strlen(const char *str)
{
  400260:	55                   	push   rbp
  400261:	48 89 e5             	mov    rbp,rsp
  400264:	48 89 7d f8          	mov    QWORD PTR [rbp-0x8],rdi
    size_t i;
    for (i = 0; str[i] != 0; i++)
  400268:	48 c7 45 f0 00 00 00 	mov    QWORD PTR [rbp-0x10],0x0
  40026f:	00 
  400270:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  400274:	48 8b 4d f0          	mov    rcx,QWORD PTR [rbp-0x10]
  400278:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  40027c:	83 fa 00             	cmp    edx,0x0
  40027f:	0f 84 16 00 00 00    	je     40029b <strlen+0x3b>
  400285:	e9 00 00 00 00       	jmp    40028a <strlen+0x2a>
  40028a:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  40028e:	48 83 c0 01          	add    rax,0x1
  400292:	48 89 45 f0          	mov    QWORD PTR [rbp-0x10],rax
  400296:	e9 d5 ff ff ff       	jmp    400270 <strlen+0x10>
        ;
    return i;
  40029b:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  40029f:	5d                   	pop    rbp
  4002a0:	c3                   	ret    
  4002a1:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
  4002a8:	00 00 00 
  4002ab:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]

00000000004002b0 <stricmp>:
}

char stricmp(const char *s1, const char *s2)
{
  4002b0:	55                   	push   rbp
  4002b1:	48 89 e5             	mov    rbp,rsp
  4002b4:	48 89 7d f8          	mov    QWORD PTR [rbp-0x8],rdi
  4002b8:	48 89 75 f0          	mov    QWORD PTR [rbp-0x10],rsi
    char diff = 0;
  4002bc:	c6 45 ef 00          	mov    BYTE PTR [rbp-0x11],0x0
    size_t i = 0;
  4002c0:	48 c7 45 e0 00 00 00 	mov    QWORD PTR [rbp-0x20],0x0
  4002c7:	00 
    do
    {
        diff = tolower(s1[i]) - tolower(s2[i]);
  4002c8:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4002cc:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  4002d0:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  4002d4:	83 fa 41             	cmp    edx,0x41
  4002d7:	0f 8c 2f 00 00 00    	jl     40030c <stricmp+0x5c>
  4002dd:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4002e1:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  4002e5:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  4002e9:	83 fa 5a             	cmp    edx,0x5a
  4002ec:	0f 8f 1a 00 00 00    	jg     40030c <stricmp+0x5c>
  4002f2:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4002f6:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  4002fa:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  4002fe:	83 c2 61             	add    edx,0x61
  400301:	83 ea 41             	sub    edx,0x41
  400304:	89 55 dc             	mov    DWORD PTR [rbp-0x24],edx
  400307:	e9 0f 00 00 00       	jmp    40031b <stricmp+0x6b>
  40030c:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  400310:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  400314:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  400318:	89 55 dc             	mov    DWORD PTR [rbp-0x24],edx
  40031b:	8b 45 dc             	mov    eax,DWORD PTR [rbp-0x24]
  40031e:	48 8b 4d f0          	mov    rcx,QWORD PTR [rbp-0x10]
  400322:	48 8b 55 e0          	mov    rdx,QWORD PTR [rbp-0x20]
  400326:	0f be 34 11          	movsx  esi,BYTE PTR [rcx+rdx*1]
  40032a:	83 fe 41             	cmp    esi,0x41
  40032d:	89 45 d8             	mov    DWORD PTR [rbp-0x28],eax
  400330:	0f 8c 2f 00 00 00    	jl     400365 <stricmp+0xb5>
  400336:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  40033a:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  40033e:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  400342:	83 fa 5a             	cmp    edx,0x5a
  400345:	0f 8f 1a 00 00 00    	jg     400365 <stricmp+0xb5>
  40034b:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  40034f:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  400353:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  400357:	83 c2 61             	add    edx,0x61
  40035a:	83 ea 41             	sub    edx,0x41
  40035d:	89 55 d4             	mov    DWORD PTR [rbp-0x2c],edx
  400360:	e9 0f 00 00 00       	jmp    400374 <stricmp+0xc4>
  400365:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  400369:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  40036d:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  400371:	89 55 d4             	mov    DWORD PTR [rbp-0x2c],edx
  400374:	8b 45 d4             	mov    eax,DWORD PTR [rbp-0x2c]
  400377:	8b 4d d8             	mov    ecx,DWORD PTR [rbp-0x28]
  40037a:	29 c1                	sub    ecx,eax
  40037c:	88 ca                	mov    dl,cl
  40037e:	88 55 ef             	mov    BYTE PTR [rbp-0x11],dl
        if (diff)
  400381:	80 7d ef 00          	cmp    BYTE PTR [rbp-0x11],0x0
  400385:	0f 84 05 00 00 00    	je     400390 <stricmp+0xe0>
            break;
  40038b:	e9 4e 00 00 00       	jmp    4003de <stricmp+0x12e>
    } while (s1[i] != 0 && s2[i++]);
  400390:	e9 00 00 00 00       	jmp    400395 <stricmp+0xe5>
  400395:	31 c0                	xor    eax,eax
  400397:	88 c1                	mov    cl,al
  400399:	48 8b 55 f8          	mov    rdx,QWORD PTR [rbp-0x8]
  40039d:	48 8b 75 e0          	mov    rsi,QWORD PTR [rbp-0x20]
  4003a1:	0f be 04 32          	movsx  eax,BYTE PTR [rdx+rsi*1]
  4003a5:	83 f8 00             	cmp    eax,0x0
  4003a8:	88 4d d3             	mov    BYTE PTR [rbp-0x2d],cl
  4003ab:	0f 84 22 00 00 00    	je     4003d3 <stricmp+0x123>
  4003b1:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  4003b5:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  4003b9:	48 89 ca             	mov    rdx,rcx
  4003bc:	48 83 c2 01          	add    rdx,0x1
  4003c0:	48 89 55 e0          	mov    QWORD PTR [rbp-0x20],rdx
  4003c4:	0f be 34 08          	movsx  esi,BYTE PTR [rax+rcx*1]
  4003c8:	83 fe 00             	cmp    esi,0x0
  4003cb:	40 0f 95 c7          	setne  dil
  4003cf:	40 88 7d d3          	mov    BYTE PTR [rbp-0x2d],dil
  4003d3:	8a 45 d3             	mov    al,BYTE PTR [rbp-0x2d]
  4003d6:	a8 01                	test   al,0x1
  4003d8:	0f 85 ea fe ff ff    	jne    4002c8 <stricmp+0x18>
    return diff;
  4003de:	0f be 45 ef          	movsx  eax,BYTE PTR [rbp-0x11]
  4003e2:	5d                   	pop    rbp
  4003e3:	c3                   	ret    
  4003e4:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
  4003eb:	00 00 00 
  4003ee:	66 90                	xchg   ax,ax

00000000004003f0 <strcmp>:
}

int strcmp(const char *s1, const char *s2)
{
  4003f0:	55                   	push   rbp
  4003f1:	48 89 e5             	mov    rbp,rsp
  4003f4:	48 89 7d f0          	mov    QWORD PTR [rbp-0x10],rdi
  4003f8:	48 89 75 e8          	mov    QWORD PTR [rbp-0x18],rsi
    size_t i = 0;
  4003fc:	48 c7 45 e0 00 00 00 	mov    QWORD PTR [rbp-0x20],0x0
  400403:	00 
    do
    {
        if ((s1[i] - s2[i]) != 0)
  400404:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  400408:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  40040c:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  400410:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  400414:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  400418:	0f be 34 08          	movsx  esi,BYTE PTR [rax+rcx*1]
  40041c:	29 f2                	sub    edx,esi
  40041e:	83 fa 00             	cmp    edx,0x0
  400421:	0f 84 29 00 00 00    	je     400450 <strcmp+0x60>
            return (char)(s1[i] - s2[i]);
  400427:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  40042b:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  40042f:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  400433:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  400437:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  40043b:	0f be 34 08          	movsx  esi,BYTE PTR [rax+rcx*1]
  40043f:	29 f2                	sub    edx,esi
  400441:	40 88 d7             	mov    dil,dl
  400444:	40 0f be d7          	movsx  edx,dil
  400448:	89 55 fc             	mov    DWORD PTR [rbp-0x4],edx
  40044b:	e9 65 00 00 00       	jmp    4004b5 <strcmp+0xc5>
    } while ((s1[i] != 0) && (s2[i] != 0) && ++i);
  400450:	e9 00 00 00 00       	jmp    400455 <strcmp+0x65>
  400455:	31 c0                	xor    eax,eax
  400457:	88 c1                	mov    cl,al
  400459:	48 8b 55 f0          	mov    rdx,QWORD PTR [rbp-0x10]
  40045d:	48 8b 75 e0          	mov    rsi,QWORD PTR [rbp-0x20]
  400461:	0f be 04 32          	movsx  eax,BYTE PTR [rdx+rsi*1]
  400465:	83 f8 00             	cmp    eax,0x0
  400468:	88 4d df             	mov    BYTE PTR [rbp-0x21],cl
  40046b:	0f 84 32 00 00 00    	je     4004a3 <strcmp+0xb3>
  400471:	31 c0                	xor    eax,eax
  400473:	88 c1                	mov    cl,al
  400475:	48 8b 55 e8          	mov    rdx,QWORD PTR [rbp-0x18]
  400479:	48 8b 75 e0          	mov    rsi,QWORD PTR [rbp-0x20]
  40047d:	0f be 04 32          	movsx  eax,BYTE PTR [rdx+rsi*1]
  400481:	83 f8 00             	cmp    eax,0x0
  400484:	88 4d df             	mov    BYTE PTR [rbp-0x21],cl
  400487:	0f 84 16 00 00 00    	je     4004a3 <strcmp+0xb3>
  40048d:	48 8b 45 e0          	mov    rax,QWORD PTR [rbp-0x20]
  400491:	48 83 c0 01          	add    rax,0x1
  400495:	48 89 45 e0          	mov    QWORD PTR [rbp-0x20],rax
  400499:	48 83 f8 00          	cmp    rax,0x0
  40049d:	0f 95 c1             	setne  cl
  4004a0:	88 4d df             	mov    BYTE PTR [rbp-0x21],cl
  4004a3:	8a 45 df             	mov    al,BYTE PTR [rbp-0x21]
  4004a6:	a8 01                	test   al,0x1
  4004a8:	0f 85 56 ff ff ff    	jne    400404 <strcmp+0x14>
    return 0;
  4004ae:	c7 45 fc 00 00 00 00 	mov    DWORD PTR [rbp-0x4],0x0
}
  4004b5:	8b 45 fc             	mov    eax,DWORD PTR [rbp-0x4]
  4004b8:	5d                   	pop    rbp
  4004b9:	c3                   	ret    
  4004ba:	66 0f 1f 44 00 00    	nop    WORD PTR [rax+rax*1+0x0]

00000000004004c0 <strstr>:

char *strstr(const char *str1, const char *str2)
{
  4004c0:	55                   	push   rbp
  4004c1:	48 89 e5             	mov    rbp,rsp
  4004c4:	48 83 ec 20          	sub    rsp,0x20
  4004c8:	48 89 7d f0          	mov    QWORD PTR [rbp-0x10],rdi
  4004cc:	48 89 75 e8          	mov    QWORD PTR [rbp-0x18],rsi
    for (int i = 0; str1[i] != 0; i++)
  4004d0:	c7 45 e4 00 00 00 00 	mov    DWORD PTR [rbp-0x1c],0x0
  4004d7:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  4004db:	48 63 4d e4          	movsxd rcx,DWORD PTR [rbp-0x1c]
  4004df:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  4004e3:	83 fa 00             	cmp    edx,0x0
  4004e6:	0f 84 47 00 00 00    	je     400533 <strstr+0x73>
    {
        if (strcmp(&str1[i], str2) == 0)
  4004ec:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  4004f0:	48 63 4d e4          	movsxd rcx,DWORD PTR [rbp-0x1c]
  4004f4:	48 01 c8             	add    rax,rcx
  4004f7:	48 8b 75 e8          	mov    rsi,QWORD PTR [rbp-0x18]
  4004fb:	48 89 c7             	mov    rdi,rax
  4004fe:	e8 ed fe ff ff       	call   4003f0 <strcmp>
  400503:	83 f8 00             	cmp    eax,0x0
  400506:	0f 85 14 00 00 00    	jne    400520 <strstr+0x60>
        {
            return (char *)&str1[i];
  40050c:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  400510:	48 63 4d e4          	movsxd rcx,DWORD PTR [rbp-0x1c]
  400514:	48 01 c8             	add    rax,rcx
  400517:	48 89 45 f8          	mov    QWORD PTR [rbp-0x8],rax
  40051b:	e9 1b 00 00 00       	jmp    40053b <strstr+0x7b>
        }
    }
  400520:	e9 00 00 00 00       	jmp    400525 <strstr+0x65>
    for (int i = 0; str1[i] != 0; i++)
  400525:	8b 45 e4             	mov    eax,DWORD PTR [rbp-0x1c]
  400528:	83 c0 01             	add    eax,0x1
  40052b:	89 45 e4             	mov    DWORD PTR [rbp-0x1c],eax
  40052e:	e9 a4 ff ff ff       	jmp    4004d7 <strstr+0x17>
    return NULL;
  400533:	48 c7 45 f8 00 00 00 	mov    QWORD PTR [rbp-0x8],0x0
  40053a:	00 
}
  40053b:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  40053f:	48 83 c4 20          	add    rsp,0x20
  400543:	5d                   	pop    rbp
  400544:	c3                   	ret    
  400545:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
  40054c:	00 00 00 
  40054f:	90                   	nop

0000000000400550 <memcmp>:

int memcmp(const void *ptr1, const void *ptr2, size_t num)
{
  400550:	55                   	push   rbp
  400551:	48 89 e5             	mov    rbp,rsp
  400554:	48 89 7d f8          	mov    QWORD PTR [rbp-0x8],rdi
  400558:	48 89 75 f0          	mov    QWORD PTR [rbp-0x10],rsi
  40055c:	48 89 55 e8          	mov    QWORD PTR [rbp-0x18],rdx
    int ret = 0;
  400560:	c7 45 e4 00 00 00 00 	mov    DWORD PTR [rbp-0x1c],0x0
    for (size_t i = 0; i < num; i++)
  400567:	48 c7 45 d8 00 00 00 	mov    QWORD PTR [rbp-0x28],0x0
  40056e:	00 
  40056f:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  400573:	48 3b 45 e8          	cmp    rax,QWORD PTR [rbp-0x18]
  400577:	0f 83 4e 00 00 00    	jae    4005cb <memcmp+0x7b>
    {
        ret = ((char *)ptr1)[i] != ((char *)ptr2)[i];
  40057d:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  400581:	48 8b 4d d8          	mov    rcx,QWORD PTR [rbp-0x28]
  400585:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  400589:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  40058d:	48 8b 4d d8          	mov    rcx,QWORD PTR [rbp-0x28]
  400591:	0f be 34 08          	movsx  esi,BYTE PTR [rax+rcx*1]
  400595:	39 f2                	cmp    edx,esi
  400597:	40 0f 95 c7          	setne  dil
  40059b:	40 80 e7 01          	and    dil,0x1
  40059f:	40 0f b6 d7          	movzx  edx,dil
  4005a3:	89 55 e4             	mov    DWORD PTR [rbp-0x1c],edx
        if (ret != 0)
  4005a6:	83 7d e4 00          	cmp    DWORD PTR [rbp-0x1c],0x0
  4005aa:	0f 84 05 00 00 00    	je     4005b5 <memcmp+0x65>
        {
            break;
  4005b0:	e9 16 00 00 00       	jmp    4005cb <memcmp+0x7b>
        }
    }
  4005b5:	e9 00 00 00 00       	jmp    4005ba <memcmp+0x6a>
    for (size_t i = 0; i < num; i++)
  4005ba:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  4005be:	48 83 c0 01          	add    rax,0x1
  4005c2:	48 89 45 d8          	mov    QWORD PTR [rbp-0x28],rax
  4005c6:	e9 a4 ff ff ff       	jmp    40056f <memcmp+0x1f>
    return ret;
  4005cb:	8b 45 e4             	mov    eax,DWORD PTR [rbp-0x1c]
  4005ce:	5d                   	pop    rbp
  4005cf:	c3                   	ret    

00000000004005d0 <strcpy>:
}

char *strcpy(char *dst, const char *src)
{
  4005d0:	55                   	push   rbp
  4005d1:	48 89 e5             	mov    rbp,rsp
  4005d4:	48 89 7d f8          	mov    QWORD PTR [rbp-0x8],rdi
  4005d8:	48 89 75 f0          	mov    QWORD PTR [rbp-0x10],rsi
    int i = 0;
  4005dc:	c7 45 ec 00 00 00 00 	mov    DWORD PTR [rbp-0x14],0x0
    for (i = 0; src[i] != 0; i++)
  4005e3:	c7 45 ec 00 00 00 00 	mov    DWORD PTR [rbp-0x14],0x0
  4005ea:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  4005ee:	48 63 4d ec          	movsxd rcx,DWORD PTR [rbp-0x14]
  4005f2:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  4005f6:	83 fa 00             	cmp    edx,0x0
  4005f9:	0f 84 24 00 00 00    	je     400623 <strcpy+0x53>
    {
        dst[i] = src[i];
  4005ff:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  400603:	48 63 4d ec          	movsxd rcx,DWORD PTR [rbp-0x14]
  400607:	8a 14 08             	mov    dl,BYTE PTR [rax+rcx*1]
  40060a:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  40060e:	48 63 4d ec          	movsxd rcx,DWORD PTR [rbp-0x14]
  400612:	88 14 08             	mov    BYTE PTR [rax+rcx*1],dl
    for (i = 0; src[i] != 0; i++)
  400615:	8b 45 ec             	mov    eax,DWORD PTR [rbp-0x14]
  400618:	83 c0 01             	add    eax,0x1
  40061b:	89 45 ec             	mov    DWORD PTR [rbp-0x14],eax
  40061e:	e9 c7 ff ff ff       	jmp    4005ea <strcpy+0x1a>
    }
    dst[i] = 0;
  400623:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  400627:	48 63 4d ec          	movsxd rcx,DWORD PTR [rbp-0x14]
  40062b:	c6 04 08 00          	mov    BYTE PTR [rax+rcx*1],0x0
    return &dst[i];
  40062f:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  400633:	48 63 4d ec          	movsxd rcx,DWORD PTR [rbp-0x14]
  400637:	48 01 c8             	add    rax,rcx
  40063a:	5d                   	pop    rbp
  40063b:	c3                   	ret    
  40063c:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]

0000000000400640 <strcat>:
}

char *strcat(char *dst, const char *src)
{
  400640:	55                   	push   rbp
  400641:	48 89 e5             	mov    rbp,rsp
  400644:	48 89 7d f8          	mov    QWORD PTR [rbp-0x8],rdi
  400648:	48 89 75 f0          	mov    QWORD PTR [rbp-0x10],rsi
    for (; dst[0] != 0; dst++)
  40064c:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  400650:	0f be 08             	movsx  ecx,BYTE PTR [rax]
  400653:	83 f9 00             	cmp    ecx,0x0
  400656:	0f 84 16 00 00 00    	je     400672 <strcat+0x32>
  40065c:	e9 00 00 00 00       	jmp    400661 <strcat+0x21>
  400661:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  400665:	48 83 c0 01          	add    rax,0x1
  400669:	48 89 45 f8          	mov    QWORD PTR [rbp-0x8],rax
  40066d:	e9 da ff ff ff       	jmp    40064c <strcat+0xc>
        ;
    int i = 0;
  400672:	c7 45 ec 00 00 00 00 	mov    DWORD PTR [rbp-0x14],0x0
    for (i = 0; src[i] != 0; i++)
  400679:	c7 45 ec 00 00 00 00 	mov    DWORD PTR [rbp-0x14],0x0
  400680:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  400684:	48 63 4d ec          	movsxd rcx,DWORD PTR [rbp-0x14]
  400688:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  40068c:	83 fa 00             	cmp    edx,0x0
  40068f:	0f 84 24 00 00 00    	je     4006b9 <strcat+0x79>
    {
        dst[i] = src[i];
  400695:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  400699:	48 63 4d ec          	movsxd rcx,DWORD PTR [rbp-0x14]
  40069d:	8a 14 08             	mov    dl,BYTE PTR [rax+rcx*1]
  4006a0:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4006a4:	48 63 4d ec          	movsxd rcx,DWORD PTR [rbp-0x14]
  4006a8:	88 14 08             	mov    BYTE PTR [rax+rcx*1],dl
    for (i = 0; src[i] != 0; i++)
  4006ab:	8b 45 ec             	mov    eax,DWORD PTR [rbp-0x14]
  4006ae:	83 c0 01             	add    eax,0x1
  4006b1:	89 45 ec             	mov    DWORD PTR [rbp-0x14],eax
  4006b4:	e9 c7 ff ff ff       	jmp    400680 <strcat+0x40>
    }
    dst[i] = 0;
  4006b9:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4006bd:	48 63 4d ec          	movsxd rcx,DWORD PTR [rbp-0x14]
  4006c1:	c6 04 08 00          	mov    BYTE PTR [rax+rcx*1],0x0
    return &dst[i];
  4006c5:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4006c9:	48 63 4d ec          	movsxd rcx,DWORD PTR [rbp-0x14]
  4006cd:	48 01 c8             	add    rax,rcx
  4006d0:	5d                   	pop    rbp
  4006d1:	c3                   	ret    
  4006d2:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
  4006d9:	00 00 00 
  4006dc:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]

00000000004006e0 <strrchar>:
}

const char *strrchar(const char *s1, int character)
{
  4006e0:	55                   	push   rbp
  4006e1:	48 89 e5             	mov    rbp,rsp
  4006e4:	48 83 ec 20          	sub    rsp,0x20
  4006e8:	48 89 7d f0          	mov    QWORD PTR [rbp-0x10],rdi
  4006ec:	89 75 ec             	mov    DWORD PTR [rbp-0x14],esi
    for (size_t len = strlen(s1); len >= 0; len--)
  4006ef:	48 8b 7d f0          	mov    rdi,QWORD PTR [rbp-0x10]
  4006f3:	e8 68 fb ff ff       	call   400260 <strlen>
  4006f8:	48 89 45 e0          	mov    QWORD PTR [rbp-0x20],rax
  4006fc:	48 83 7d e0 00       	cmp    QWORD PTR [rbp-0x20],0x0
  400701:	0f 82 3c 00 00 00    	jb     400743 <strrchar+0x63>
    {
        if (s1[len] == character)
  400707:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  40070b:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  40070f:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  400713:	3b 55 ec             	cmp    edx,DWORD PTR [rbp-0x14]
  400716:	0f 85 11 00 00 00    	jne    40072d <strrchar+0x4d>
        {
            return &s1[len];
  40071c:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  400720:	48 03 45 e0          	add    rax,QWORD PTR [rbp-0x20]
  400724:	48 89 45 f8          	mov    QWORD PTR [rbp-0x8],rax
  400728:	e9 1e 00 00 00       	jmp    40074b <strrchar+0x6b>
        }
    }
  40072d:	e9 00 00 00 00       	jmp    400732 <strrchar+0x52>
    for (size_t len = strlen(s1); len >= 0; len--)
  400732:	48 8b 45 e0          	mov    rax,QWORD PTR [rbp-0x20]
  400736:	48 83 c0 ff          	add    rax,0xffffffffffffffff
  40073a:	48 89 45 e0          	mov    QWORD PTR [rbp-0x20],rax
  40073e:	e9 b9 ff ff ff       	jmp    4006fc <strrchar+0x1c>
    return NULL;
  400743:	48 c7 45 f8 00 00 00 	mov    QWORD PTR [rbp-0x8],0x0
  40074a:	00 
}
  40074b:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  40074f:	48 83 c4 20          	add    rsp,0x20
  400753:	5d                   	pop    rbp
  400754:	c3                   	ret    
  400755:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
  40075c:	00 00 00 
  40075f:	90                   	nop

0000000000400760 <strchar>:

const char *strchar(const char *s1, int character)
{
  400760:	55                   	push   rbp
  400761:	48 89 e5             	mov    rbp,rsp
  400764:	48 89 7d f0          	mov    QWORD PTR [rbp-0x10],rdi
  400768:	89 75 ec             	mov    DWORD PTR [rbp-0x14],esi
    for (size_t len = 0; s1[len]; len++)
  40076b:	48 c7 45 e0 00 00 00 	mov    QWORD PTR [rbp-0x20],0x0
  400772:	00 
  400773:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  400777:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  40077b:	80 3c 08 00          	cmp    BYTE PTR [rax+rcx*1],0x0
  40077f:	0f 84 3c 00 00 00    	je     4007c1 <strchar+0x61>
    {
        if (s1[len] == character)
  400785:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  400789:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  40078d:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  400791:	3b 55 ec             	cmp    edx,DWORD PTR [rbp-0x14]
  400794:	0f 85 11 00 00 00    	jne    4007ab <strchar+0x4b>
        {
            return &s1[len];
  40079a:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  40079e:	48 03 45 e0          	add    rax,QWORD PTR [rbp-0x20]
  4007a2:	48 89 45 f8          	mov    QWORD PTR [rbp-0x8],rax
  4007a6:	e9 1e 00 00 00       	jmp    4007c9 <strchar+0x69>
        }
    }
  4007ab:	e9 00 00 00 00       	jmp    4007b0 <strchar+0x50>
    for (size_t len = 0; s1[len]; len++)
  4007b0:	48 8b 45 e0          	mov    rax,QWORD PTR [rbp-0x20]
  4007b4:	48 83 c0 01          	add    rax,0x1
  4007b8:	48 89 45 e0          	mov    QWORD PTR [rbp-0x20],rax
  4007bc:	e9 b2 ff ff ff       	jmp    400773 <strchar+0x13>
    return NULL;
  4007c1:	48 c7 45 f8 00 00 00 	mov    QWORD PTR [rbp-0x8],0x0
  4007c8:	00 
}
  4007c9:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4007cd:	5d                   	pop    rbp
  4007ce:	c3                   	ret    
  4007cf:	90                   	nop

00000000004007d0 <strncpy>:

char *strncpy(char *dst, const char *src, size_t count)
{
  4007d0:	55                   	push   rbp
  4007d1:	48 89 e5             	mov    rbp,rsp
  4007d4:	48 89 7d f8          	mov    QWORD PTR [rbp-0x8],rdi
  4007d8:	48 89 75 f0          	mov    QWORD PTR [rbp-0x10],rsi
  4007dc:	48 89 55 e8          	mov    QWORD PTR [rbp-0x18],rdx
    int srcEnded = 0;
  4007e0:	c7 45 e4 00 00 00 00 	mov    DWORD PTR [rbp-0x1c],0x0
    for (size_t i = 0; i < count; i++)
  4007e7:	48 c7 45 d8 00 00 00 	mov    QWORD PTR [rbp-0x28],0x0
  4007ee:	00 
  4007ef:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  4007f3:	48 3b 45 e8          	cmp    rax,QWORD PTR [rbp-0x18]
  4007f7:	0f 83 6c 00 00 00    	jae    400869 <strncpy+0x99>
    {
        if (srcEnded)
  4007fd:	83 7d e4 00          	cmp    DWORD PTR [rbp-0x1c],0x0
  400801:	0f 84 0c 00 00 00    	je     400813 <strncpy+0x43>
        {
            *dst = 0;
  400807:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  40080b:	c6 00 00             	mov    BYTE PTR [rax],0x0
        }
  40080e:	e9 39 00 00 00       	jmp    40084c <strncpy+0x7c>
        else
        {
            *dst = *src;
  400813:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  400817:	8a 08                	mov    cl,BYTE PTR [rax]
  400819:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  40081d:	88 08                	mov    BYTE PTR [rax],cl
            if (*src == 0)
  40081f:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  400823:	0f be 10             	movsx  edx,BYTE PTR [rax]
  400826:	83 fa 00             	cmp    edx,0x0
  400829:	0f 85 0c 00 00 00    	jne    40083b <strncpy+0x6b>
                srcEnded = 1;
  40082f:	c7 45 e4 01 00 00 00 	mov    DWORD PTR [rbp-0x1c],0x1
  400836:	e9 0c 00 00 00       	jmp    400847 <strncpy+0x77>
            else
                src++;
  40083b:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  40083f:	48 83 c0 01          	add    rax,0x1
  400843:	48 89 45 f0          	mov    QWORD PTR [rbp-0x10],rax
  400847:	e9 00 00 00 00       	jmp    40084c <strncpy+0x7c>
        }
        dst++;
  40084c:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  400850:	48 83 c0 01          	add    rax,0x1
  400854:	48 89 45 f8          	mov    QWORD PTR [rbp-0x8],rax
    for (size_t i = 0; i < count; i++)
  400858:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  40085c:	48 83 c0 01          	add    rax,0x1
  400860:	48 89 45 d8          	mov    QWORD PTR [rbp-0x28],rax
  400864:	e9 86 ff ff ff       	jmp    4007ef <strncpy+0x1f>
    }
    return dst;
  400869:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  40086d:	5d                   	pop    rbp
  40086e:	c3                   	ret    
  40086f:	90                   	nop

0000000000400870 <strncmp>:
}

int strncmp(const char *str1, const char *str2, size_t num)
{
  400870:	55                   	push   rbp
  400871:	48 89 e5             	mov    rbp,rsp
  400874:	48 89 7d f0          	mov    QWORD PTR [rbp-0x10],rdi
  400878:	48 89 75 e8          	mov    QWORD PTR [rbp-0x18],rsi
  40087c:	48 89 55 e0          	mov    QWORD PTR [rbp-0x20],rdx
    for (size_t i = 0; i < num; i++)
  400880:	48 c7 45 d8 00 00 00 	mov    QWORD PTR [rbp-0x28],0x0
  400887:	00 
  400888:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  40088c:	48 3b 45 e0          	cmp    rax,QWORD PTR [rbp-0x20]
  400890:	0f 83 85 00 00 00    	jae    40091b <strncmp+0xab>
    {
        if ((str1[i] == 0) || (str2[i] == 0) || ((str1[i] - str2[i]) != 0))
  400896:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  40089a:	48 8b 4d d8          	mov    rcx,QWORD PTR [rbp-0x28]
  40089e:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  4008a2:	83 fa 00             	cmp    edx,0x0
  4008a5:	0f 84 38 00 00 00    	je     4008e3 <strncmp+0x73>
  4008ab:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  4008af:	48 8b 4d d8          	mov    rcx,QWORD PTR [rbp-0x28]
  4008b3:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  4008b7:	83 fa 00             	cmp    edx,0x0
  4008ba:	0f 84 23 00 00 00    	je     4008e3 <strncmp+0x73>
  4008c0:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  4008c4:	48 8b 4d d8          	mov    rcx,QWORD PTR [rbp-0x28]
  4008c8:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  4008cc:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  4008d0:	48 8b 4d d8          	mov    rcx,QWORD PTR [rbp-0x28]
  4008d4:	0f be 34 08          	movsx  esi,BYTE PTR [rax+rcx*1]
  4008d8:	29 f2                	sub    edx,esi
  4008da:	83 fa 00             	cmp    edx,0x0
  4008dd:	0f 84 22 00 00 00    	je     400905 <strncmp+0x95>
        {
            return (str1[i] - str2[i]);
  4008e3:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  4008e7:	48 8b 4d d8          	mov    rcx,QWORD PTR [rbp-0x28]
  4008eb:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  4008ef:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  4008f3:	48 8b 4d d8          	mov    rcx,QWORD PTR [rbp-0x28]
  4008f7:	0f be 34 08          	movsx  esi,BYTE PTR [rax+rcx*1]
  4008fb:	29 f2                	sub    edx,esi
  4008fd:	89 55 fc             	mov    DWORD PTR [rbp-0x4],edx
  400900:	e9 1d 00 00 00       	jmp    400922 <strncmp+0xb2>
        }
    }
  400905:	e9 00 00 00 00       	jmp    40090a <strncmp+0x9a>
    for (size_t i = 0; i < num; i++)
  40090a:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  40090e:	48 83 c0 01          	add    rax,0x1
  400912:	48 89 45 d8          	mov    QWORD PTR [rbp-0x28],rax
  400916:	e9 6d ff ff ff       	jmp    400888 <strncmp+0x18>
    return 0;
  40091b:	c7 45 fc 00 00 00 00 	mov    DWORD PTR [rbp-0x4],0x0
}
  400922:	8b 45 fc             	mov    eax,DWORD PTR [rbp-0x4]
  400925:	5d                   	pop    rbp
  400926:	c3                   	ret    
  400927:	66 0f 1f 84 00 00 00 	nop    WORD PTR [rax+rax*1+0x0]
  40092e:	00 00 

0000000000400930 <strncat>:

char *strncat(char *destination, const char *source, size_t num)
{
  400930:	55                   	push   rbp
  400931:	48 89 e5             	mov    rbp,rsp
  400934:	48 89 7d f8          	mov    QWORD PTR [rbp-0x8],rdi
  400938:	48 89 75 f0          	mov    QWORD PTR [rbp-0x10],rsi
  40093c:	48 89 55 e8          	mov    QWORD PTR [rbp-0x18],rdx
    for (; *destination; destination++)
  400940:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  400944:	80 38 00             	cmp    BYTE PTR [rax],0x0
  400947:	0f 84 16 00 00 00    	je     400963 <strncat+0x33>
  40094d:	e9 00 00 00 00       	jmp    400952 <strncat+0x22>
  400952:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  400956:	48 83 c0 01          	add    rax,0x1
  40095a:	48 89 45 f8          	mov    QWORD PTR [rbp-0x8],rax
  40095e:	e9 dd ff ff ff       	jmp    400940 <strncat+0x10>
        ;
    for (size_t i = 0; i < num && source[i] != 0; i++)
  400963:	48 c7 45 e0 00 00 00 	mov    QWORD PTR [rbp-0x20],0x0
  40096a:	00 
  40096b:	31 c0                	xor    eax,eax
  40096d:	88 c1                	mov    cl,al
  40096f:	48 8b 55 e0          	mov    rdx,QWORD PTR [rbp-0x20]
  400973:	48 3b 55 e8          	cmp    rdx,QWORD PTR [rbp-0x18]
  400977:	88 4d df             	mov    BYTE PTR [rbp-0x21],cl
  40097a:	0f 83 17 00 00 00    	jae    400997 <strncat+0x67>
  400980:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  400984:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  400988:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  40098c:	83 fa 00             	cmp    edx,0x0
  40098f:	40 0f 95 c6          	setne  sil
  400993:	40 88 75 df          	mov    BYTE PTR [rbp-0x21],sil
  400997:	8a 45 df             	mov    al,BYTE PTR [rbp-0x21]
  40099a:	a8 01                	test   al,0x1
  40099c:	0f 85 05 00 00 00    	jne    4009a7 <strncat+0x77>
  4009a2:	e9 2d 00 00 00       	jmp    4009d4 <strncat+0xa4>
    {
        *destination++ = source[i];
  4009a7:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  4009ab:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  4009af:	8a 14 08             	mov    dl,BYTE PTR [rax+rcx*1]
  4009b2:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4009b6:	48 89 c1             	mov    rcx,rax
  4009b9:	48 83 c1 01          	add    rcx,0x1
  4009bd:	48 89 4d f8          	mov    QWORD PTR [rbp-0x8],rcx
  4009c1:	88 10                	mov    BYTE PTR [rax],dl
    for (size_t i = 0; i < num && source[i] != 0; i++)
  4009c3:	48 8b 45 e0          	mov    rax,QWORD PTR [rbp-0x20]
  4009c7:	48 83 c0 01          	add    rax,0x1
  4009cb:	48 89 45 e0          	mov    QWORD PTR [rbp-0x20],rax
  4009cf:	e9 97 ff ff ff       	jmp    40096b <strncat+0x3b>
    }
    *destination = 0;
  4009d4:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4009d8:	c6 00 00             	mov    BYTE PTR [rax],0x0
    return destination;
  4009db:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4009df:	5d                   	pop    rbp
  4009e0:	c3                   	ret    
  4009e1:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
  4009e8:	00 00 00 
  4009eb:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]

00000000004009f0 <strtok>:
}

char *strtok(char *str, const char *delimiters)
{
  4009f0:	55                   	push   rbp
  4009f1:	48 89 e5             	mov    rbp,rsp
  4009f4:	48 89 7d f0          	mov    QWORD PTR [rbp-0x10],rdi
  4009f8:	48 89 75 e8          	mov    QWORD PTR [rbp-0x18],rsi
    static char *ptr = NULL;
    if (str != NULL)
  4009fc:	48 83 7d f0 00       	cmp    QWORD PTR [rbp-0x10],0x0
  400a01:	0f 84 0c 00 00 00    	je     400a13 <strtok+0x23>
    {
        ptr = str;
  400a07:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  400a0b:	48 89 04 25 00 30 60 	mov    QWORD PTR ds:0x603000,rax
  400a12:	00 
    }
    if (!ptr || *ptr == 0)
  400a13:	48 83 3c 25 00 30 60 	cmp    QWORD PTR ds:0x603000,0x0
  400a1a:	00 00 
  400a1c:	0f 84 14 00 00 00    	je     400a36 <strtok+0x46>
  400a22:	48 8b 04 25 00 30 60 	mov    rax,QWORD PTR ds:0x603000
  400a29:	00 
  400a2a:	0f be 08             	movsx  ecx,BYTE PTR [rax]
  400a2d:	83 f9 00             	cmp    ecx,0x0
  400a30:	0f 85 0d 00 00 00    	jne    400a43 <strtok+0x53>
    {
        return NULL;
  400a36:	48 c7 45 f8 00 00 00 	mov    QWORD PTR [rbp-0x8],0x0
  400a3d:	00 
  400a3e:	e9 ff 00 00 00       	jmp    400b42 <strtok+0x152>
    }
    char *result = ptr;
  400a43:	48 8b 04 25 00 30 60 	mov    rax,QWORD PTR ds:0x603000
  400a4a:	00 
  400a4b:	48 89 45 e0          	mov    QWORD PTR [rbp-0x20],rax
    int delimFound = 0;
  400a4f:	c7 45 dc 00 00 00 00 	mov    DWORD PTR [rbp-0x24],0x0
  400a56:	31 c0                	xor    eax,eax
  400a58:	88 c1                	mov    cl,al
    while (*ptr != 0 && !delimFound)
  400a5a:	48 8b 14 25 00 30 60 	mov    rdx,QWORD PTR ds:0x603000
  400a61:	00 
  400a62:	0f be 02             	movsx  eax,BYTE PTR [rdx]
  400a65:	83 f8 00             	cmp    eax,0x0
  400a68:	88 4d cf             	mov    BYTE PTR [rbp-0x31],cl
  400a6b:	0f 84 0c 00 00 00    	je     400a7d <strtok+0x8d>
  400a71:	83 7d dc 00          	cmp    DWORD PTR [rbp-0x24],0x0
  400a75:	0f 95 c0             	setne  al
  400a78:	34 ff                	xor    al,0xff
  400a7a:	88 45 cf             	mov    BYTE PTR [rbp-0x31],al
  400a7d:	8a 45 cf             	mov    al,BYTE PTR [rbp-0x31]
  400a80:	a8 01                	test   al,0x1
  400a82:	0f 85 05 00 00 00    	jne    400a8d <strtok+0x9d>
  400a88:	e9 82 00 00 00       	jmp    400b0f <strtok+0x11f>
    {
        for (size_t i = 0; delimiters[i] != 0; i++)
  400a8d:	48 c7 45 d0 00 00 00 	mov    QWORD PTR [rbp-0x30],0x0
  400a94:	00 
  400a95:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  400a99:	48 8b 4d d0          	mov    rcx,QWORD PTR [rbp-0x30]
  400a9d:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  400aa1:	83 fa 00             	cmp    edx,0x0
  400aa4:	0f 84 4c 00 00 00    	je     400af6 <strtok+0x106>
        {
            if (*ptr == delimiters[i])
  400aaa:	48 8b 04 25 00 30 60 	mov    rax,QWORD PTR ds:0x603000
  400ab1:	00 
  400ab2:	0f be 08             	movsx  ecx,BYTE PTR [rax]
  400ab5:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  400ab9:	48 8b 55 d0          	mov    rdx,QWORD PTR [rbp-0x30]
  400abd:	0f be 34 10          	movsx  esi,BYTE PTR [rax+rdx*1]
  400ac1:	39 f1                	cmp    ecx,esi
  400ac3:	0f 85 17 00 00 00    	jne    400ae0 <strtok+0xf0>
            {
                *ptr = 0;
  400ac9:	48 8b 04 25 00 30 60 	mov    rax,QWORD PTR ds:0x603000
  400ad0:	00 
  400ad1:	c6 00 00             	mov    BYTE PTR [rax],0x0
                delimFound = 1;
  400ad4:	c7 45 dc 01 00 00 00 	mov    DWORD PTR [rbp-0x24],0x1
                break;
  400adb:	e9 16 00 00 00       	jmp    400af6 <strtok+0x106>
            }
        }
  400ae0:	e9 00 00 00 00       	jmp    400ae5 <strtok+0xf5>
        for (size_t i = 0; delimiters[i] != 0; i++)
  400ae5:	48 8b 45 d0          	mov    rax,QWORD PTR [rbp-0x30]
  400ae9:	48 83 c0 01          	add    rax,0x1
  400aed:	48 89 45 d0          	mov    QWORD PTR [rbp-0x30],rax
  400af1:	e9 9f ff ff ff       	jmp    400a95 <strtok+0xa5>
        ++ptr;
  400af6:	48 8b 04 25 00 30 60 	mov    rax,QWORD PTR ds:0x603000
  400afd:	00 
  400afe:	48 83 c0 01          	add    rax,0x1
  400b02:	48 89 04 25 00 30 60 	mov    QWORD PTR ds:0x603000,rax
  400b09:	00 
    while (*ptr != 0 && !delimFound)
  400b0a:	e9 47 ff ff ff       	jmp    400a56 <strtok+0x66>
    }

    if (!delimFound && *ptr != 0)
  400b0f:	83 7d dc 00          	cmp    DWORD PTR [rbp-0x24],0x0
  400b13:	0f 85 21 00 00 00    	jne    400b3a <strtok+0x14a>
  400b19:	48 8b 04 25 00 30 60 	mov    rax,QWORD PTR ds:0x603000
  400b20:	00 
  400b21:	0f be 08             	movsx  ecx,BYTE PTR [rax]
  400b24:	83 f9 00             	cmp    ecx,0x0
  400b27:	0f 84 0d 00 00 00    	je     400b3a <strtok+0x14a>
    {
        return NULL;
  400b2d:	48 c7 45 f8 00 00 00 	mov    QWORD PTR [rbp-0x8],0x0
  400b34:	00 
  400b35:	e9 08 00 00 00       	jmp    400b42 <strtok+0x152>
    }

    return result;
  400b3a:	48 8b 45 e0          	mov    rax,QWORD PTR [rbp-0x20]
  400b3e:	48 89 45 f8          	mov    QWORD PTR [rbp-0x8],rax
}
  400b42:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  400b46:	5d                   	pop    rbp
  400b47:	c3                   	ret    
  400b48:	0f 1f 84 00 00 00 00 	nop    DWORD PTR [rax+rax*1+0x0]
  400b4f:	00 

0000000000400b50 <putchar>:
	}
	return c;
}
#else
int putchar(int c)
{
  400b50:	55                   	push   rbp
  400b51:	48 89 e5             	mov    rbp,rsp
  400b54:	89 7d fc             	mov    DWORD PTR [rbp-0x4],edi
	__asm__ volatile("syscall" ::"D"(SYSCALL_PUTCHAR), "S"(c)
  400b57:	8b 75 fc             	mov    esi,DWORD PTR [rbp-0x4]
  400b5a:	31 ff                	xor    edi,edi
  400b5c:	0f 05                	syscall 
					 : "rcx", "r11");
	return c;
  400b5e:	8b 45 fc             	mov    eax,DWORD PTR [rbp-0x4]
  400b61:	5d                   	pop    rbp
  400b62:	c3                   	ret    
  400b63:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
  400b6a:	00 00 00 
  400b6d:	0f 1f 00             	nop    DWORD PTR [rax]

0000000000400b70 <getchar>:
}
char getchar()
{
  400b70:	55                   	push   rbp
  400b71:	48 89 e5             	mov    rbp,rsp
	char ch = 0;
  400b74:	c6 45 ff 00          	mov    BYTE PTR [rbp-0x1],0x0
  400b78:	bf 01 00 00 00       	mov    edi,0x1
  400b7d:	48 8d 75 ff          	lea    rsi,[rbp-0x1]
	__asm__ volatile("syscall" ::"D"(SYSCALL_GETCHAR), "S"(&ch)
  400b81:	0f 05                	syscall 
					 : "rcx", "r11");
	return ch;
  400b83:	0f be 45 ff          	movsx  eax,BYTE PTR [rbp-0x1]
  400b87:	5d                   	pop    rbp
  400b88:	c3                   	ret    
  400b89:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]

0000000000400b90 <printf>:
		}
	}
}

int printf(const char *format, ...)
{
  400b90:	55                   	push   rbp
  400b91:	48 89 e5             	mov    rbp,rsp
  400b94:	48 81 ec 40 02 00 00 	sub    rsp,0x240
  400b9b:	84 c0                	test   al,al
  400b9d:	0f 29 bd f0 fe ff ff 	movaps XMMWORD PTR [rbp-0x110],xmm7
  400ba4:	0f 29 b5 e0 fe ff ff 	movaps XMMWORD PTR [rbp-0x120],xmm6
  400bab:	0f 29 ad d0 fe ff ff 	movaps XMMWORD PTR [rbp-0x130],xmm5
  400bb2:	0f 29 a5 c0 fe ff ff 	movaps XMMWORD PTR [rbp-0x140],xmm4
  400bb9:	0f 29 9d b0 fe ff ff 	movaps XMMWORD PTR [rbp-0x150],xmm3
  400bc0:	0f 29 95 a0 fe ff ff 	movaps XMMWORD PTR [rbp-0x160],xmm2
  400bc7:	0f 29 8d 90 fe ff ff 	movaps XMMWORD PTR [rbp-0x170],xmm1
  400bce:	0f 29 85 80 fe ff ff 	movaps XMMWORD PTR [rbp-0x180],xmm0
  400bd5:	48 89 bd 78 fe ff ff 	mov    QWORD PTR [rbp-0x188],rdi
  400bdc:	4c 89 8d 70 fe ff ff 	mov    QWORD PTR [rbp-0x190],r9
  400be3:	4c 89 85 68 fe ff ff 	mov    QWORD PTR [rbp-0x198],r8
  400bea:	48 89 8d 60 fe ff ff 	mov    QWORD PTR [rbp-0x1a0],rcx
  400bf1:	48 89 95 58 fe ff ff 	mov    QWORD PTR [rbp-0x1a8],rdx
  400bf8:	48 89 b5 50 fe ff ff 	mov    QWORD PTR [rbp-0x1b0],rsi
  400bff:	0f 84 67 00 00 00    	je     400c6c <printf+0xdc>
  400c05:	0f 28 85 80 fe ff ff 	movaps xmm0,XMMWORD PTR [rbp-0x180]
  400c0c:	0f 29 85 30 ff ff ff 	movaps XMMWORD PTR [rbp-0xd0],xmm0
  400c13:	0f 28 8d 90 fe ff ff 	movaps xmm1,XMMWORD PTR [rbp-0x170]
  400c1a:	0f 29 8d 40 ff ff ff 	movaps XMMWORD PTR [rbp-0xc0],xmm1
  400c21:	0f 28 95 a0 fe ff ff 	movaps xmm2,XMMWORD PTR [rbp-0x160]
  400c28:	0f 29 95 50 ff ff ff 	movaps XMMWORD PTR [rbp-0xb0],xmm2
  400c2f:	0f 28 9d b0 fe ff ff 	movaps xmm3,XMMWORD PTR [rbp-0x150]
  400c36:	0f 29 9d 60 ff ff ff 	movaps XMMWORD PTR [rbp-0xa0],xmm3
  400c3d:	0f 28 a5 c0 fe ff ff 	movaps xmm4,XMMWORD PTR [rbp-0x140]
  400c44:	0f 29 a5 70 ff ff ff 	movaps XMMWORD PTR [rbp-0x90],xmm4
  400c4b:	0f 28 ad d0 fe ff ff 	movaps xmm5,XMMWORD PTR [rbp-0x130]
  400c52:	0f 29 6d 80          	movaps XMMWORD PTR [rbp-0x80],xmm5
  400c56:	0f 28 b5 e0 fe ff ff 	movaps xmm6,XMMWORD PTR [rbp-0x120]
  400c5d:	0f 29 75 90          	movaps XMMWORD PTR [rbp-0x70],xmm6
  400c61:	0f 28 bd f0 fe ff ff 	movaps xmm7,XMMWORD PTR [rbp-0x110]
  400c68:	0f 29 7d a0          	movaps XMMWORD PTR [rbp-0x60],xmm7
  400c6c:	48 8b 85 70 fe ff ff 	mov    rax,QWORD PTR [rbp-0x190]
  400c73:	48 89 85 28 ff ff ff 	mov    QWORD PTR [rbp-0xd8],rax
  400c7a:	48 8b 8d 68 fe ff ff 	mov    rcx,QWORD PTR [rbp-0x198]
  400c81:	48 89 8d 20 ff ff ff 	mov    QWORD PTR [rbp-0xe0],rcx
  400c88:	48 8b 95 60 fe ff ff 	mov    rdx,QWORD PTR [rbp-0x1a0]
  400c8f:	48 89 95 18 ff ff ff 	mov    QWORD PTR [rbp-0xe8],rdx
  400c96:	48 8b b5 58 fe ff ff 	mov    rsi,QWORD PTR [rbp-0x1a8]
  400c9d:	48 89 b5 10 ff ff ff 	mov    QWORD PTR [rbp-0xf0],rsi
  400ca4:	48 8b bd 50 fe ff ff 	mov    rdi,QWORD PTR [rbp-0x1b0]
  400cab:	48 89 bd 08 ff ff ff 	mov    QWORD PTR [rbp-0xf8],rdi
  400cb2:	4c 8b 85 78 fe ff ff 	mov    r8,QWORD PTR [rbp-0x188]
  400cb9:	4c 8d 4d e0          	lea    r9,[rbp-0x20]
  400cbd:	4c 89 45 f8          	mov    QWORD PTR [rbp-0x8],r8
  400cc1:	4c 8d 85 00 ff ff ff 	lea    r8,[rbp-0x100]
	va_list ap;
	va_start(ap, format);
  400cc8:	4d 89 41 10          	mov    QWORD PTR [r9+0x10],r8
  400ccc:	4c 8d 45 10          	lea    r8,[rbp+0x10]
  400cd0:	4d 89 41 08          	mov    QWORD PTR [r9+0x8],r8
  400cd4:	41 c7 41 04 30 00 00 	mov    DWORD PTR [r9+0x4],0x30
  400cdb:	00 
  400cdc:	41 c7 01 08 00 00 00 	mov    DWORD PTR [r9],0x8
	for (uint64_t i = 0; format[i] != 0; i++)
  400ce3:	48 c7 45 d8 00 00 00 	mov    QWORD PTR [rbp-0x28],0x0
  400cea:	00 
  400ceb:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  400cef:	48 8b 4d d8          	mov    rcx,QWORD PTR [rbp-0x28]
  400cf3:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  400cf7:	83 fa 00             	cmp    edx,0x0
  400cfa:	0f 84 27 03 00 00    	je     401027 <printf+0x497>
	{
		switch (format[i])
  400d00:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  400d04:	48 8b 4d d8          	mov    rcx,QWORD PTR [rbp-0x28]
  400d08:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  400d0c:	89 d6                	mov    esi,edx
  400d0e:	83 c6 f7             	add    esi,0xfffffff7
  400d11:	83 ee 02             	sub    esi,0x2
  400d14:	89 95 4c fe ff ff    	mov    DWORD PTR [rbp-0x1b4],edx
  400d1a:	89 b5 48 fe ff ff    	mov    DWORD PTR [rbp-0x1b8],esi
  400d20:	0f 82 cf 02 00 00    	jb     400ff5 <printf+0x465>
  400d26:	e9 00 00 00 00       	jmp    400d2b <printf+0x19b>
  400d2b:	8b 85 4c fe ff ff    	mov    eax,DWORD PTR [rbp-0x1b4]
  400d31:	83 e8 0d             	sub    eax,0xd
  400d34:	89 85 44 fe ff ff    	mov    DWORD PTR [rbp-0x1bc],eax
  400d3a:	0f 84 b5 02 00 00    	je     400ff5 <printf+0x465>
  400d40:	e9 00 00 00 00       	jmp    400d45 <printf+0x1b5>
  400d45:	8b 85 4c fe ff ff    	mov    eax,DWORD PTR [rbp-0x1b4]
  400d4b:	83 e8 25             	sub    eax,0x25
  400d4e:	89 85 40 fe ff ff    	mov    DWORD PTR [rbp-0x1c0],eax
  400d54:	0f 85 a0 02 00 00    	jne    400ffa <printf+0x46a>
  400d5a:	e9 00 00 00 00       	jmp    400d5f <printf+0x1cf>
		{
		case '%':
			i++;
  400d5f:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  400d63:	48 83 c0 01          	add    rax,0x1
  400d67:	48 89 45 d8          	mov    QWORD PTR [rbp-0x28],rax
			switch (format[i])
  400d6b:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  400d6f:	48 8b 4d d8          	mov    rcx,QWORD PTR [rbp-0x28]
  400d73:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  400d77:	83 c2 9c             	add    edx,0xffffff9c
  400d7a:	89 d0                	mov    eax,edx
  400d7c:	83 ea 14             	sub    edx,0x14
  400d7f:	48 89 85 38 fe ff ff 	mov    QWORD PTR [rbp-0x1c8],rax
  400d86:	89 95 34 fe ff ff    	mov    DWORD PTR [rbp-0x1cc],edx
  400d8c:	0f 87 5e 02 00 00    	ja     400ff0 <printf+0x460>
  400d92:	48 8b 85 38 fe ff ff 	mov    rax,QWORD PTR [rbp-0x1c8]
  400d99:	48 8b 0c c5 48 26 40 	mov    rcx,QWORD PTR [rax*8+0x402648]
  400da0:	00 
  400da1:	ff e1                	jmp    rcx
			{
			case 'l':
			{
				i++;
  400da3:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  400da7:	48 83 c0 01          	add    rax,0x1
  400dab:	48 89 45 d8          	mov    QWORD PTR [rbp-0x28],rax
				if (format[i] == 'x')
  400daf:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  400db3:	48 8b 4d d8          	mov    rcx,QWORD PTR [rbp-0x28]
  400db7:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  400dbb:	83 fa 78             	cmp    edx,0x78
  400dbe:	0f 85 7b 00 00 00    	jne    400e3f <printf+0x2af>
  400dc4:	48 8d 45 e0          	lea    rax,[rbp-0x20]
				{
					uint64_t arg = va_arg(ap, uint64_t);
  400dc8:	8b 4d e0             	mov    ecx,DWORD PTR [rbp-0x20]
  400dcb:	83 f9 28             	cmp    ecx,0x28
  400dce:	48 89 85 28 fe ff ff 	mov    QWORD PTR [rbp-0x1d8],rax
  400dd5:	89 8d 24 fe ff ff    	mov    DWORD PTR [rbp-0x1dc],ecx
  400ddb:	0f 87 25 00 00 00    	ja     400e06 <printf+0x276>
  400de1:	8b 85 24 fe ff ff    	mov    eax,DWORD PTR [rbp-0x1dc]
  400de7:	48 63 c8             	movsxd rcx,eax
  400dea:	48 8b 95 28 fe ff ff 	mov    rdx,QWORD PTR [rbp-0x1d8]
  400df1:	48 03 4a 10          	add    rcx,QWORD PTR [rdx+0x10]
  400df5:	83 c0 08             	add    eax,0x8
  400df8:	89 02                	mov    DWORD PTR [rdx],eax
  400dfa:	48 89 8d 18 fe ff ff 	mov    QWORD PTR [rbp-0x1e8],rcx
  400e01:	e9 1d 00 00 00       	jmp    400e23 <printf+0x293>
  400e06:	48 8b 85 28 fe ff ff 	mov    rax,QWORD PTR [rbp-0x1d8]
  400e0d:	48 8b 48 08          	mov    rcx,QWORD PTR [rax+0x8]
  400e11:	48 89 ca             	mov    rdx,rcx
  400e14:	48 83 c1 08          	add    rcx,0x8
  400e18:	48 89 48 08          	mov    QWORD PTR [rax+0x8],rcx
  400e1c:	48 89 95 18 fe ff ff 	mov    QWORD PTR [rbp-0x1e8],rdx
  400e23:	48 8b 85 18 fe ff ff 	mov    rax,QWORD PTR [rbp-0x1e8]
  400e2a:	48 8b 00             	mov    rax,QWORD PTR [rax]
  400e2d:	48 89 45 d0          	mov    QWORD PTR [rbp-0x30],rax
					print_num64(arg, 16);
  400e31:	48 8b 7d d0          	mov    rdi,QWORD PTR [rbp-0x30]
  400e35:	be 10 00 00 00       	mov    esi,0x10
  400e3a:	e8 01 02 00 00       	call   401040 <print_num64>
				}
				break;
  400e3f:	e9 ac 01 00 00       	jmp    400ff0 <printf+0x460>
  400e44:	48 8d 45 e0          	lea    rax,[rbp-0x20]
			}
			case 'x':
			{
				uint64_t arg = va_arg(ap, uint64_t);
  400e48:	8b 4d e0             	mov    ecx,DWORD PTR [rbp-0x20]
  400e4b:	83 f9 28             	cmp    ecx,0x28
  400e4e:	48 89 85 10 fe ff ff 	mov    QWORD PTR [rbp-0x1f0],rax
  400e55:	89 8d 0c fe ff ff    	mov    DWORD PTR [rbp-0x1f4],ecx
  400e5b:	0f 87 25 00 00 00    	ja     400e86 <printf+0x2f6>
  400e61:	8b 85 0c fe ff ff    	mov    eax,DWORD PTR [rbp-0x1f4]
  400e67:	48 63 c8             	movsxd rcx,eax
  400e6a:	48 8b 95 10 fe ff ff 	mov    rdx,QWORD PTR [rbp-0x1f0]
  400e71:	48 03 4a 10          	add    rcx,QWORD PTR [rdx+0x10]
  400e75:	83 c0 08             	add    eax,0x8
  400e78:	89 02                	mov    DWORD PTR [rdx],eax
  400e7a:	48 89 8d 00 fe ff ff 	mov    QWORD PTR [rbp-0x200],rcx
  400e81:	e9 1d 00 00 00       	jmp    400ea3 <printf+0x313>
  400e86:	48 8b 85 10 fe ff ff 	mov    rax,QWORD PTR [rbp-0x1f0]
  400e8d:	48 8b 48 08          	mov    rcx,QWORD PTR [rax+0x8]
  400e91:	48 89 ca             	mov    rdx,rcx
  400e94:	48 83 c1 08          	add    rcx,0x8
  400e98:	48 89 48 08          	mov    QWORD PTR [rax+0x8],rcx
  400e9c:	48 89 95 00 fe ff ff 	mov    QWORD PTR [rbp-0x200],rdx
  400ea3:	48 8b 85 00 fe ff ff 	mov    rax,QWORD PTR [rbp-0x200]
  400eaa:	48 8b 00             	mov    rax,QWORD PTR [rax]
  400ead:	48 89 45 c8          	mov    QWORD PTR [rbp-0x38],rax
				print_num32(arg, 16);
  400eb1:	48 8b 45 c8          	mov    rax,QWORD PTR [rbp-0x38]
  400eb5:	89 c1                	mov    ecx,eax
  400eb7:	89 cf                	mov    edi,ecx
  400eb9:	be 10 00 00 00       	mov    esi,0x10
  400ebe:	e8 bd 02 00 00       	call   401180 <print_num32>
				break;
  400ec3:	e9 28 01 00 00       	jmp    400ff0 <printf+0x460>
  400ec8:	48 8d 45 e0          	lea    rax,[rbp-0x20]
			}
			case 'd':
			{
				uint64_t arg = va_arg(ap, uint64_t);
  400ecc:	8b 4d e0             	mov    ecx,DWORD PTR [rbp-0x20]
  400ecf:	83 f9 28             	cmp    ecx,0x28
  400ed2:	48 89 85 f8 fd ff ff 	mov    QWORD PTR [rbp-0x208],rax
  400ed9:	89 8d f4 fd ff ff    	mov    DWORD PTR [rbp-0x20c],ecx
  400edf:	0f 87 25 00 00 00    	ja     400f0a <printf+0x37a>
  400ee5:	8b 85 f4 fd ff ff    	mov    eax,DWORD PTR [rbp-0x20c]
  400eeb:	48 63 c8             	movsxd rcx,eax
  400eee:	48 8b 95 f8 fd ff ff 	mov    rdx,QWORD PTR [rbp-0x208]
  400ef5:	48 03 4a 10          	add    rcx,QWORD PTR [rdx+0x10]
  400ef9:	83 c0 08             	add    eax,0x8
  400efc:	89 02                	mov    DWORD PTR [rdx],eax
  400efe:	48 89 8d e8 fd ff ff 	mov    QWORD PTR [rbp-0x218],rcx
  400f05:	e9 1d 00 00 00       	jmp    400f27 <printf+0x397>
  400f0a:	48 8b 85 f8 fd ff ff 	mov    rax,QWORD PTR [rbp-0x208]
  400f11:	48 8b 48 08          	mov    rcx,QWORD PTR [rax+0x8]
  400f15:	48 89 ca             	mov    rdx,rcx
  400f18:	48 83 c1 08          	add    rcx,0x8
  400f1c:	48 89 48 08          	mov    QWORD PTR [rax+0x8],rcx
  400f20:	48 89 95 e8 fd ff ff 	mov    QWORD PTR [rbp-0x218],rdx
  400f27:	48 8b 85 e8 fd ff ff 	mov    rax,QWORD PTR [rbp-0x218]
  400f2e:	48 8b 00             	mov    rax,QWORD PTR [rax]
  400f31:	48 89 45 c0          	mov    QWORD PTR [rbp-0x40],rax
				print_num32(arg, 10);
  400f35:	48 8b 45 c0          	mov    rax,QWORD PTR [rbp-0x40]
  400f39:	89 c1                	mov    ecx,eax
  400f3b:	89 cf                	mov    edi,ecx
  400f3d:	be 0a 00 00 00       	mov    esi,0xa
  400f42:	e8 39 02 00 00       	call   401180 <print_num32>
				break;
  400f47:	e9 a4 00 00 00       	jmp    400ff0 <printf+0x460>
  400f4c:	48 8d 45 e0          	lea    rax,[rbp-0x20]
			}
			case 's':
			{
				char *str = va_arg(ap, char *);
  400f50:	8b 4d e0             	mov    ecx,DWORD PTR [rbp-0x20]
  400f53:	83 f9 28             	cmp    ecx,0x28
  400f56:	48 89 85 e0 fd ff ff 	mov    QWORD PTR [rbp-0x220],rax
  400f5d:	89 8d dc fd ff ff    	mov    DWORD PTR [rbp-0x224],ecx
  400f63:	0f 87 25 00 00 00    	ja     400f8e <printf+0x3fe>
  400f69:	8b 85 dc fd ff ff    	mov    eax,DWORD PTR [rbp-0x224]
  400f6f:	48 63 c8             	movsxd rcx,eax
  400f72:	48 8b 95 e0 fd ff ff 	mov    rdx,QWORD PTR [rbp-0x220]
  400f79:	48 03 4a 10          	add    rcx,QWORD PTR [rdx+0x10]
  400f7d:	83 c0 08             	add    eax,0x8
  400f80:	89 02                	mov    DWORD PTR [rdx],eax
  400f82:	48 89 8d d0 fd ff ff 	mov    QWORD PTR [rbp-0x230],rcx
  400f89:	e9 1d 00 00 00       	jmp    400fab <printf+0x41b>
  400f8e:	48 8b 85 e0 fd ff ff 	mov    rax,QWORD PTR [rbp-0x220]
  400f95:	48 8b 48 08          	mov    rcx,QWORD PTR [rax+0x8]
  400f99:	48 89 ca             	mov    rdx,rcx
  400f9c:	48 83 c1 08          	add    rcx,0x8
  400fa0:	48 89 48 08          	mov    QWORD PTR [rax+0x8],rcx
  400fa4:	48 89 95 d0 fd ff ff 	mov    QWORD PTR [rbp-0x230],rdx
  400fab:	48 8b 85 d0 fd ff ff 	mov    rax,QWORD PTR [rbp-0x230]
  400fb2:	48 8b 00             	mov    rax,QWORD PTR [rax]
  400fb5:	48 89 45 b8          	mov    QWORD PTR [rbp-0x48],rax
				while (str[0] != 0)
  400fb9:	48 8b 45 b8          	mov    rax,QWORD PTR [rbp-0x48]
  400fbd:	0f be 08             	movsx  ecx,BYTE PTR [rax]
  400fc0:	83 f9 00             	cmp    ecx,0x0
  400fc3:	0f 84 22 00 00 00    	je     400feb <printf+0x45b>
				{
					putchar(*str++);
  400fc9:	48 8b 45 b8          	mov    rax,QWORD PTR [rbp-0x48]
  400fcd:	48 89 c1             	mov    rcx,rax
  400fd0:	48 83 c1 01          	add    rcx,0x1
  400fd4:	48 89 4d b8          	mov    QWORD PTR [rbp-0x48],rcx
  400fd8:	0f be 38             	movsx  edi,BYTE PTR [rax]
  400fdb:	e8 70 fb ff ff       	call   400b50 <putchar>
				while (str[0] != 0)
  400fe0:	89 85 cc fd ff ff    	mov    DWORD PTR [rbp-0x234],eax
  400fe6:	e9 ce ff ff ff       	jmp    400fb9 <printf+0x429>
				}
				break;
  400feb:	e9 00 00 00 00       	jmp    400ff0 <printf+0x460>
			}
			}
			break;
  400ff0:	e9 1c 00 00 00       	jmp    401011 <printf+0x481>
  400ff5:	e9 00 00 00 00       	jmp    400ffa <printf+0x46a>
		case '\n':
		case '\r':
		case '\t':
		default:
			putchar(format[i]);
  400ffa:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  400ffe:	48 8b 4d d8          	mov    rcx,QWORD PTR [rbp-0x28]
  401002:	0f be 3c 08          	movsx  edi,BYTE PTR [rax+rcx*1]
  401006:	e8 45 fb ff ff       	call   400b50 <putchar>
  40100b:	89 85 c8 fd ff ff    	mov    DWORD PTR [rbp-0x238],eax
			break;
		}
	}
  401011:	e9 00 00 00 00       	jmp    401016 <printf+0x486>
	for (uint64_t i = 0; format[i] != 0; i++)
  401016:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  40101a:	48 83 c0 01          	add    rax,0x1
  40101e:	48 89 45 d8          	mov    QWORD PTR [rbp-0x28],rax
  401022:	e9 c4 fc ff ff       	jmp    400ceb <printf+0x15b>
  401027:	31 c0                	xor    eax,eax
	va_end(ap);
	return 0;
  401029:	48 81 c4 40 02 00 00 	add    rsp,0x240
  401030:	5d                   	pop    rbp
  401031:	c3                   	ret    
  401032:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
  401039:	00 00 00 
  40103c:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]

0000000000401040 <print_num64>:
{
  401040:	55                   	push   rbp
  401041:	48 89 e5             	mov    rbp,rsp
  401044:	48 81 ec a0 00 00 00 	sub    rsp,0xa0
  40104b:	b8 25 00 00 00       	mov    eax,0x25
  401050:	89 c2                	mov    edx,eax
  401052:	31 c0                	xor    eax,eax
  401054:	b9 18 00 00 00       	mov    ecx,0x18
  401059:	41 89 c8             	mov    r8d,ecx
  40105c:	48 89 7d f8          	mov    QWORD PTR [rbp-0x8],rdi
  401060:	89 75 f4             	mov    DWORD PTR [rbp-0xc],esi
	char str[24] = {0};
  401063:	48 8d 7d d0          	lea    rdi,[rbp-0x30]
  401067:	89 c6                	mov    esi,eax
  401069:	48 89 55 80          	mov    QWORD PTR [rbp-0x80],rdx
  40106d:	4c 89 c2             	mov    rdx,r8
  401070:	e8 db f0 ff ff       	call   400150 <memset>
	str[23] = '0';
  401075:	c6 45 e7 30          	mov    BYTE PTR [rbp-0x19],0x30
	char symbol[] = {"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
  401079:	48 8d 55 a0          	lea    rdx,[rbp-0x60]
  40107d:	48 89 d7             	mov    rdi,rdx
  401080:	48 be 00 27 40 00 00 	movabs rsi,0x402700
  401087:	00 00 00 
  40108a:	48 8b 55 80          	mov    rdx,QWORD PTR [rbp-0x80]
  40108e:	e8 0d f1 ff ff       	call   4001a0 <memcpy>
	for (uint64_t i = arg, no = 23; (i > 0) && (no >= 0); i /= base, no--)
  401093:	48 8b 55 f8          	mov    rdx,QWORD PTR [rbp-0x8]
  401097:	48 89 55 98          	mov    QWORD PTR [rbp-0x68],rdx
  40109b:	48 c7 45 90 17 00 00 	mov    QWORD PTR [rbp-0x70],0x17
  4010a2:	00 
  4010a3:	31 c0                	xor    eax,eax
  4010a5:	88 c1                	mov    cl,al
  4010a7:	48 83 7d 98 00       	cmp    QWORD PTR [rbp-0x68],0x0
  4010ac:	88 8d 7f ff ff ff    	mov    BYTE PTR [rbp-0x81],cl
  4010b2:	0f 86 0e 00 00 00    	jbe    4010c6 <print_num64+0x86>
  4010b8:	48 83 7d 90 00       	cmp    QWORD PTR [rbp-0x70],0x0
  4010bd:	0f 93 c0             	setae  al
  4010c0:	88 85 7f ff ff ff    	mov    BYTE PTR [rbp-0x81],al
  4010c6:	8a 85 7f ff ff ff    	mov    al,BYTE PTR [rbp-0x81]
  4010cc:	a8 01                	test   al,0x1
  4010ce:	0f 85 05 00 00 00    	jne    4010d9 <print_num64+0x99>
  4010d4:	e9 4f 00 00 00       	jmp    401128 <print_num64+0xe8>
		str[no] = symbol[(i % base)];
  4010d9:	48 8b 45 98          	mov    rax,QWORD PTR [rbp-0x68]
  4010dd:	8b 4d f4             	mov    ecx,DWORD PTR [rbp-0xc]
  4010e0:	89 ca                	mov    edx,ecx
  4010e2:	31 c9                	xor    ecx,ecx
  4010e4:	48 89 95 70 ff ff ff 	mov    QWORD PTR [rbp-0x90],rdx
  4010eb:	89 ca                	mov    edx,ecx
  4010ed:	48 8b b5 70 ff ff ff 	mov    rsi,QWORD PTR [rbp-0x90]
  4010f4:	48 f7 f6             	div    rsi
  4010f7:	40 8a 7c 15 a0       	mov    dil,BYTE PTR [rbp+rdx*1-0x60]
  4010fc:	48 8b 55 90          	mov    rdx,QWORD PTR [rbp-0x70]
  401100:	40 88 7c 15 d0       	mov    BYTE PTR [rbp+rdx*1-0x30],dil
	for (uint64_t i = arg, no = 23; (i > 0) && (no >= 0); i /= base, no--)
  401105:	8b 45 f4             	mov    eax,DWORD PTR [rbp-0xc]
  401108:	89 c1                	mov    ecx,eax
  40110a:	48 8b 45 98          	mov    rax,QWORD PTR [rbp-0x68]
  40110e:	31 d2                	xor    edx,edx
  401110:	48 f7 f1             	div    rcx
  401113:	48 89 45 98          	mov    QWORD PTR [rbp-0x68],rax
  401117:	48 8b 45 90          	mov    rax,QWORD PTR [rbp-0x70]
  40111b:	48 83 c0 ff          	add    rax,0xffffffffffffffff
  40111f:	48 89 45 90          	mov    QWORD PTR [rbp-0x70],rax
  401123:	e9 7b ff ff ff       	jmp    4010a3 <print_num64+0x63>
	for (int i = 0; i < 24; ++i)
  401128:	c7 45 8c 00 00 00 00 	mov    DWORD PTR [rbp-0x74],0x0
  40112f:	83 7d 8c 18          	cmp    DWORD PTR [rbp-0x74],0x18
  401133:	0f 8d 39 00 00 00    	jge    401172 <print_num64+0x132>
		if (str[i] != 0)
  401139:	48 63 45 8c          	movsxd rax,DWORD PTR [rbp-0x74]
  40113d:	0f be 4c 05 d0       	movsx  ecx,BYTE PTR [rbp+rax*1-0x30]
  401142:	83 f9 00             	cmp    ecx,0x0
  401145:	0f 84 14 00 00 00    	je     40115f <print_num64+0x11f>
			putchar(str[i]);
  40114b:	48 63 45 8c          	movsxd rax,DWORD PTR [rbp-0x74]
  40114f:	0f be 7c 05 d0       	movsx  edi,BYTE PTR [rbp+rax*1-0x30]
  401154:	e8 f7 f9 ff ff       	call   400b50 <putchar>
  401159:	89 85 6c ff ff ff    	mov    DWORD PTR [rbp-0x94],eax
	}
  40115f:	e9 00 00 00 00       	jmp    401164 <print_num64+0x124>
	for (int i = 0; i < 24; ++i)
  401164:	8b 45 8c             	mov    eax,DWORD PTR [rbp-0x74]
  401167:	83 c0 01             	add    eax,0x1
  40116a:	89 45 8c             	mov    DWORD PTR [rbp-0x74],eax
  40116d:	e9 bd ff ff ff       	jmp    40112f <print_num64+0xef>
}
  401172:	48 81 c4 a0 00 00 00 	add    rsp,0xa0
  401179:	5d                   	pop    rbp
  40117a:	c3                   	ret    
  40117b:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]

0000000000401180 <print_num32>:
{
  401180:	55                   	push   rbp
  401181:	48 89 e5             	mov    rbp,rsp
  401184:	48 83 ec 50          	sub    rsp,0x50
  401188:	31 c0                	xor    eax,eax
  40118a:	b9 0c 00 00 00       	mov    ecx,0xc
  40118f:	89 ca                	mov    edx,ecx
  401191:	89 7d fc             	mov    DWORD PTR [rbp-0x4],edi
  401194:	89 75 f8             	mov    DWORD PTR [rbp-0x8],esi
	char str[12] = {0};
  401197:	4c 8d 45 ec          	lea    r8,[rbp-0x14]
  40119b:	4c 89 c7             	mov    rdi,r8
  40119e:	89 c6                	mov    esi,eax
  4011a0:	e8 ab ef ff ff       	call   400150 <memset>
	str[11] = '0';
  4011a5:	c6 45 f7 30          	mov    BYTE PTR [rbp-0x9],0x30
	char symbol[] = {"0123456789ABCDEFG"};
  4011a9:	48 8b 14 25 30 27 40 	mov    rdx,QWORD PTR ds:0x402730
  4011b0:	00 
  4011b1:	48 89 55 d0          	mov    QWORD PTR [rbp-0x30],rdx
  4011b5:	48 8b 14 25 38 27 40 	mov    rdx,QWORD PTR ds:0x402738
  4011bc:	00 
  4011bd:	48 89 55 d8          	mov    QWORD PTR [rbp-0x28],rdx
  4011c1:	66 44 8b 0c 25 40 27 	mov    r9w,WORD PTR ds:0x402740
  4011c8:	40 00 
  4011ca:	66 44 89 4d e0       	mov    WORD PTR [rbp-0x20],r9w
	for (uint32_t i = arg, no = 11; (i > 0) && (no >= 0); i /= base, no--)
  4011cf:	8b 45 fc             	mov    eax,DWORD PTR [rbp-0x4]
  4011d2:	89 45 cc             	mov    DWORD PTR [rbp-0x34],eax
  4011d5:	c7 45 c8 0b 00 00 00 	mov    DWORD PTR [rbp-0x38],0xb
  4011dc:	31 c0                	xor    eax,eax
  4011de:	88 c1                	mov    cl,al
  4011e0:	83 7d cc 00          	cmp    DWORD PTR [rbp-0x34],0x0
  4011e4:	88 4d c3             	mov    BYTE PTR [rbp-0x3d],cl
  4011e7:	0f 86 0a 00 00 00    	jbe    4011f7 <print_num32+0x77>
  4011ed:	83 7d c8 00          	cmp    DWORD PTR [rbp-0x38],0x0
  4011f1:	0f 93 c0             	setae  al
  4011f4:	88 45 c3             	mov    BYTE PTR [rbp-0x3d],al
  4011f7:	8a 45 c3             	mov    al,BYTE PTR [rbp-0x3d]
  4011fa:	a8 01                	test   al,0x1
  4011fc:	0f 85 05 00 00 00    	jne    401207 <print_num32+0x87>
  401202:	e9 3e 00 00 00       	jmp    401245 <print_num32+0xc5>
		str[no] = symbol[(i % base)];
  401207:	8b 45 cc             	mov    eax,DWORD PTR [rbp-0x34]
  40120a:	31 d2                	xor    edx,edx
  40120c:	f7 75 f8             	div    DWORD PTR [rbp-0x8]
  40120f:	89 d2                	mov    edx,edx
  401211:	89 d1                	mov    ecx,edx
  401213:	40 8a 74 0d d0       	mov    sil,BYTE PTR [rbp+rcx*1-0x30]
  401218:	8b 55 c8             	mov    edx,DWORD PTR [rbp-0x38]
  40121b:	89 d1                	mov    ecx,edx
  40121d:	40 88 74 0d ec       	mov    BYTE PTR [rbp+rcx*1-0x14],sil
	for (uint32_t i = arg, no = 11; (i > 0) && (no >= 0); i /= base, no--)
  401222:	8b 45 f8             	mov    eax,DWORD PTR [rbp-0x8]
  401225:	8b 4d cc             	mov    ecx,DWORD PTR [rbp-0x34]
  401228:	89 45 bc             	mov    DWORD PTR [rbp-0x44],eax
  40122b:	89 c8                	mov    eax,ecx
  40122d:	31 d2                	xor    edx,edx
  40122f:	8b 4d bc             	mov    ecx,DWORD PTR [rbp-0x44]
  401232:	f7 f1                	div    ecx
  401234:	89 45 cc             	mov    DWORD PTR [rbp-0x34],eax
  401237:	8b 45 c8             	mov    eax,DWORD PTR [rbp-0x38]
  40123a:	83 c0 ff             	add    eax,0xffffffff
  40123d:	89 45 c8             	mov    DWORD PTR [rbp-0x38],eax
  401240:	e9 97 ff ff ff       	jmp    4011dc <print_num32+0x5c>
	for (int i = 0; i < 12; ++i)
  401245:	c7 45 c4 00 00 00 00 	mov    DWORD PTR [rbp-0x3c],0x0
  40124c:	83 7d c4 0c          	cmp    DWORD PTR [rbp-0x3c],0xc
  401250:	0f 8d 36 00 00 00    	jge    40128c <print_num32+0x10c>
		if (str[i] != 0)
  401256:	48 63 45 c4          	movsxd rax,DWORD PTR [rbp-0x3c]
  40125a:	0f be 4c 05 ec       	movsx  ecx,BYTE PTR [rbp+rax*1-0x14]
  40125f:	83 f9 00             	cmp    ecx,0x0
  401262:	0f 84 11 00 00 00    	je     401279 <print_num32+0xf9>
			putchar(str[i]);
  401268:	48 63 45 c4          	movsxd rax,DWORD PTR [rbp-0x3c]
  40126c:	0f be 7c 05 ec       	movsx  edi,BYTE PTR [rbp+rax*1-0x14]
  401271:	e8 da f8 ff ff       	call   400b50 <putchar>
  401276:	89 45 b8             	mov    DWORD PTR [rbp-0x48],eax
	}
  401279:	e9 00 00 00 00       	jmp    40127e <print_num32+0xfe>
	for (int i = 0; i < 12; ++i)
  40127e:	8b 45 c4             	mov    eax,DWORD PTR [rbp-0x3c]
  401281:	83 c0 01             	add    eax,0x1
  401284:	89 45 c4             	mov    DWORD PTR [rbp-0x3c],eax
  401287:	e9 c0 ff ff ff       	jmp    40124c <print_num32+0xcc>
}
  40128c:	48 83 c4 50          	add    rsp,0x50
  401290:	5d                   	pop    rbp
  401291:	c3                   	ret    
  401292:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
  401299:	00 00 00 
  40129c:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]

00000000004012a0 <gets_s>:
}

char *gets_s(char *str, size_t sz)
{
  4012a0:	55                   	push   rbp
  4012a1:	48 89 e5             	mov    rbp,rsp
  4012a4:	48 83 ec 20          	sub    rsp,0x20
  4012a8:	48 89 7d f8          	mov    QWORD PTR [rbp-0x8],rdi
  4012ac:	48 89 75 f0          	mov    QWORD PTR [rbp-0x10],rsi
	size_t it = 0;
  4012b0:	48 c7 45 e8 00 00 00 	mov    QWORD PTR [rbp-0x18],0x0
  4012b7:	00 
	char ch = 0;
  4012b8:	c6 45 e7 00          	mov    BYTE PTR [rbp-0x19],0x0
	do
	{
		ch = getchar();
  4012bc:	e8 af f8 ff ff       	call   400b70 <getchar>
  4012c1:	88 45 e7             	mov    BYTE PTR [rbp-0x19],al
		switch (ch)
  4012c4:	8a 45 e7             	mov    al,BYTE PTR [rbp-0x19]
  4012c7:	2c 08                	sub    al,0x8
  4012c9:	88 45 e6             	mov    BYTE PTR [rbp-0x1a],al
  4012cc:	0f 85 22 00 00 00    	jne    4012f4 <gets_s+0x54>
  4012d2:	e9 00 00 00 00       	jmp    4012d7 <gets_s+0x37>
		{
		case '\b':
			it--;
  4012d7:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  4012db:	48 83 c0 ff          	add    rax,0xffffffffffffffff
  4012df:	48 89 45 e8          	mov    QWORD PTR [rbp-0x18],rax
			str[it] = 0;
  4012e3:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4012e7:	48 8b 4d e8          	mov    rcx,QWORD PTR [rbp-0x18]
  4012eb:	c6 04 08 00          	mov    BYTE PTR [rax+rcx*1],0x0
			break;
  4012ef:	e9 1a 00 00 00       	jmp    40130e <gets_s+0x6e>
		default:
			str[it] = ch;
  4012f4:	8a 45 e7             	mov    al,BYTE PTR [rbp-0x19]
  4012f7:	48 8b 4d f8          	mov    rcx,QWORD PTR [rbp-0x8]
  4012fb:	48 8b 55 e8          	mov    rdx,QWORD PTR [rbp-0x18]
  4012ff:	88 04 11             	mov    BYTE PTR [rcx+rdx*1],al
			it++;
  401302:	48 8b 4d e8          	mov    rcx,QWORD PTR [rbp-0x18]
  401306:	48 83 c1 01          	add    rcx,0x1
  40130a:	48 89 4d e8          	mov    QWORD PTR [rbp-0x18],rcx
			break;
		}
	} while (it < sz && ch != '\n');
  40130e:	e9 00 00 00 00       	jmp    401313 <gets_s+0x73>
  401313:	31 c0                	xor    eax,eax
  401315:	88 c1                	mov    cl,al
  401317:	48 8b 55 e8          	mov    rdx,QWORD PTR [rbp-0x18]
  40131b:	48 3b 55 f0          	cmp    rdx,QWORD PTR [rbp-0x10]
  40131f:	88 4d e5             	mov    BYTE PTR [rbp-0x1b],cl
  401322:	0f 83 0d 00 00 00    	jae    401335 <gets_s+0x95>
  401328:	0f be 45 e7          	movsx  eax,BYTE PTR [rbp-0x19]
  40132c:	83 f8 0a             	cmp    eax,0xa
  40132f:	0f 95 c1             	setne  cl
  401332:	88 4d e5             	mov    BYTE PTR [rbp-0x1b],cl
  401335:	8a 45 e5             	mov    al,BYTE PTR [rbp-0x1b]
  401338:	a8 01                	test   al,0x1
  40133a:	0f 85 7c ff ff ff    	jne    4012bc <gets_s+0x1c>
	if (it >= 1)
  401340:	48 83 7d e8 01       	cmp    QWORD PTR [rbp-0x18],0x1
  401345:	0f 82 10 00 00 00    	jb     40135b <gets_s+0xbb>
		str[it - 1] = 0;
  40134b:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  40134f:	48 8b 4d e8          	mov    rcx,QWORD PTR [rbp-0x18]
  401353:	48 83 e9 01          	sub    rcx,0x1
  401357:	c6 04 08 00          	mov    BYTE PTR [rax+rcx*1],0x0
	return str;
  40135b:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  40135f:	48 83 c4 20          	add    rsp,0x20
  401363:	5d                   	pop    rbp
  401364:	c3                   	ret    
  401365:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
  40136c:	00 00 00 
  40136f:	90                   	nop

0000000000401370 <kisnum>:
}
int kisnum(const char ch)
{
  401370:	55                   	push   rbp
  401371:	48 89 e5             	mov    rbp,rsp
  401374:	40 88 f8             	mov    al,dil
  401377:	31 ff                	xor    edi,edi
  401379:	40 88 f9             	mov    cl,dil
  40137c:	88 45 ff             	mov    BYTE PTR [rbp-0x1],al
	return (ch >= '0' && ch <= '9');
  40137f:	0f be 7d ff          	movsx  edi,BYTE PTR [rbp-0x1]
  401383:	83 ff 30             	cmp    edi,0x30
  401386:	88 4d fe             	mov    BYTE PTR [rbp-0x2],cl
  401389:	0f 8c 0d 00 00 00    	jl     40139c <kisnum+0x2c>
  40138f:	0f be 45 ff          	movsx  eax,BYTE PTR [rbp-0x1]
  401393:	83 f8 39             	cmp    eax,0x39
  401396:	0f 9e c1             	setle  cl
  401399:	88 4d fe             	mov    BYTE PTR [rbp-0x2],cl
  40139c:	8a 45 fe             	mov    al,BYTE PTR [rbp-0x2]
  40139f:	24 01                	and    al,0x1
  4013a1:	0f b6 c0             	movzx  eax,al
  4013a4:	5d                   	pop    rbp
  4013a5:	c3                   	ret    
  4013a6:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
  4013ad:	00 00 00 

00000000004013b0 <max>:
	WidthFlags_j,
	WidthFlags_z,
	WidthFlags_t,
	WidthFlags_L
};
int max(int a, int b) { return (a > b) ? a : b; }
  4013b0:	55                   	push   rbp
  4013b1:	48 89 e5             	mov    rbp,rsp
  4013b4:	89 7d fc             	mov    DWORD PTR [rbp-0x4],edi
  4013b7:	89 75 f8             	mov    DWORD PTR [rbp-0x8],esi
  4013ba:	8b 75 fc             	mov    esi,DWORD PTR [rbp-0x4]
  4013bd:	3b 75 f8             	cmp    esi,DWORD PTR [rbp-0x8]
  4013c0:	0f 8e 0b 00 00 00    	jle    4013d1 <max+0x21>
  4013c6:	8b 45 fc             	mov    eax,DWORD PTR [rbp-0x4]
  4013c9:	89 45 f4             	mov    DWORD PTR [rbp-0xc],eax
  4013cc:	e9 06 00 00 00       	jmp    4013d7 <max+0x27>
  4013d1:	8b 45 f8             	mov    eax,DWORD PTR [rbp-0x8]
  4013d4:	89 45 f4             	mov    DWORD PTR [rbp-0xc],eax
  4013d7:	8b 45 f4             	mov    eax,DWORD PTR [rbp-0xc]
  4013da:	5d                   	pop    rbp
  4013db:	c3                   	ret    
  4013dc:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]

00000000004013e0 <decode_format>:
int decode_format(const char *buffer, enum FormatFlag *flags, int *width, int *precision, enum WidthFlags *length, int *specifier)
{
  4013e0:	55                   	push   rbp
  4013e1:	48 89 e5             	mov    rbp,rsp
  4013e4:	48 89 7d f8          	mov    QWORD PTR [rbp-0x8],rdi
  4013e8:	48 89 75 f0          	mov    QWORD PTR [rbp-0x10],rsi
  4013ec:	48 89 55 e8          	mov    QWORD PTR [rbp-0x18],rdx
  4013f0:	48 89 4d e0          	mov    QWORD PTR [rbp-0x20],rcx
  4013f4:	4c 89 45 d8          	mov    QWORD PTR [rbp-0x28],r8
  4013f8:	4c 89 4d d0          	mov    QWORD PTR [rbp-0x30],r9
	//%[flags][width][.precision][length]specifier
	int buffer_index = 1;
  4013fc:	c7 45 cc 01 00 00 00 	mov    DWORD PTR [rbp-0x34],0x1
	*flags = FormatFlag_None;
  401403:	48 8b 4d f0          	mov    rcx,QWORD PTR [rbp-0x10]
  401407:	c7 01 00 00 00 00    	mov    DWORD PTR [rcx],0x0
	width[0] = 0;
  40140d:	48 8b 4d e8          	mov    rcx,QWORD PTR [rbp-0x18]
  401411:	c7 01 00 00 00 00    	mov    DWORD PTR [rcx],0x0
	precision[0] = -1;
  401417:	48 8b 4d e0          	mov    rcx,QWORD PTR [rbp-0x20]
  40141b:	c7 01 ff ff ff ff    	mov    DWORD PTR [rcx],0xffffffff
	specifier[0] = 0;
  401421:	48 8b 4d d0          	mov    rcx,QWORD PTR [rbp-0x30]
  401425:	c7 01 00 00 00 00    	mov    DWORD PTR [rcx],0x0
	if (buffer[buffer_index] == '-')
  40142b:	48 8b 4d f8          	mov    rcx,QWORD PTR [rbp-0x8]
  40142f:	48 63 55 cc          	movsxd rdx,DWORD PTR [rbp-0x34]
  401433:	0f be 04 11          	movsx  eax,BYTE PTR [rcx+rdx*1]
  401437:	83 f8 2d             	cmp    eax,0x2d
  40143a:	0f 85 18 00 00 00    	jne    401458 <decode_format+0x78>
	{
		*flags = FormatFlag_LeftJustify;
  401440:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  401444:	c7 00 01 00 00 00    	mov    DWORD PTR [rax],0x1
		buffer_index++;
  40144a:	8b 4d cc             	mov    ecx,DWORD PTR [rbp-0x34]
  40144d:	83 c1 01             	add    ecx,0x1
  401450:	89 4d cc             	mov    DWORD PTR [rbp-0x34],ecx
	}
  401453:	e9 c3 00 00 00       	jmp    40151b <decode_format+0x13b>
	else if (buffer[buffer_index] == '+')
  401458:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  40145c:	48 63 4d cc          	movsxd rcx,DWORD PTR [rbp-0x34]
  401460:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  401464:	83 fa 2b             	cmp    edx,0x2b
  401467:	0f 85 18 00 00 00    	jne    401485 <decode_format+0xa5>
	{
		*flags = FormatFlag_PrependSign;
  40146d:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  401471:	c7 00 02 00 00 00    	mov    DWORD PTR [rax],0x2
		buffer_index++;
  401477:	8b 4d cc             	mov    ecx,DWORD PTR [rbp-0x34]
  40147a:	83 c1 01             	add    ecx,0x1
  40147d:	89 4d cc             	mov    DWORD PTR [rbp-0x34],ecx
	}
  401480:	e9 91 00 00 00       	jmp    401516 <decode_format+0x136>
	else if (buffer[buffer_index] == ' ')
  401485:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  401489:	48 63 4d cc          	movsxd rcx,DWORD PTR [rbp-0x34]
  40148d:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  401491:	83 fa 20             	cmp    edx,0x20
  401494:	0f 85 18 00 00 00    	jne    4014b2 <decode_format+0xd2>
	{
		*flags = FormatFlag_BlankIfNoSign;
  40149a:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  40149e:	c7 00 03 00 00 00    	mov    DWORD PTR [rax],0x3
		buffer_index++;
  4014a4:	8b 4d cc             	mov    ecx,DWORD PTR [rbp-0x34]
  4014a7:	83 c1 01             	add    ecx,0x1
  4014aa:	89 4d cc             	mov    DWORD PTR [rbp-0x34],ecx
	}
  4014ad:	e9 5f 00 00 00       	jmp    401511 <decode_format+0x131>
	else if (buffer[buffer_index] == '#')
  4014b2:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4014b6:	48 63 4d cc          	movsxd rcx,DWORD PTR [rbp-0x34]
  4014ba:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  4014be:	83 fa 23             	cmp    edx,0x23
  4014c1:	0f 85 18 00 00 00    	jne    4014df <decode_format+0xff>
	{
		*flags = FormatFlag_PrependBaseNotifier;
  4014c7:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  4014cb:	c7 00 04 00 00 00    	mov    DWORD PTR [rax],0x4
		buffer_index++;
  4014d1:	8b 4d cc             	mov    ecx,DWORD PTR [rbp-0x34]
  4014d4:	83 c1 01             	add    ecx,0x1
  4014d7:	89 4d cc             	mov    DWORD PTR [rbp-0x34],ecx
	}
  4014da:	e9 2d 00 00 00       	jmp    40150c <decode_format+0x12c>
	else if (buffer[buffer_index] == '0')
  4014df:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4014e3:	48 63 4d cc          	movsxd rcx,DWORD PTR [rbp-0x34]
  4014e7:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  4014eb:	83 fa 30             	cmp    edx,0x30
  4014ee:	0f 85 13 00 00 00    	jne    401507 <decode_format+0x127>
	{
		*flags = FormatFlag_PadZeros;
  4014f4:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  4014f8:	c7 00 05 00 00 00    	mov    DWORD PTR [rax],0x5
		buffer_index++;
  4014fe:	8b 4d cc             	mov    ecx,DWORD PTR [rbp-0x34]
  401501:	83 c1 01             	add    ecx,0x1
  401504:	89 4d cc             	mov    DWORD PTR [rbp-0x34],ecx
  401507:	e9 00 00 00 00       	jmp    40150c <decode_format+0x12c>
  40150c:	e9 00 00 00 00       	jmp    401511 <decode_format+0x131>
  401511:	e9 00 00 00 00       	jmp    401516 <decode_format+0x136>
  401516:	e9 00 00 00 00       	jmp    40151b <decode_format+0x13b>
	}
	while (isdigit(buffer[buffer_index]))
  40151b:	e9 00 00 00 00       	jmp    401520 <decode_format+0x140>
  401520:	31 c0                	xor    eax,eax
  401522:	88 c1                	mov    cl,al
  401524:	48 8b 55 f8          	mov    rdx,QWORD PTR [rbp-0x8]
  401528:	48 63 75 cc          	movsxd rsi,DWORD PTR [rbp-0x34]
  40152c:	0f be 04 32          	movsx  eax,BYTE PTR [rdx+rsi*1]
  401530:	83 f8 30             	cmp    eax,0x30
  401533:	88 4d cb             	mov    BYTE PTR [rbp-0x35],cl
  401536:	0f 8c 17 00 00 00    	jl     401553 <decode_format+0x173>
  40153c:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  401540:	48 63 4d cc          	movsxd rcx,DWORD PTR [rbp-0x34]
  401544:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  401548:	83 fa 39             	cmp    edx,0x39
  40154b:	40 0f 9e c6          	setle  sil
  40154f:	40 88 75 cb          	mov    BYTE PTR [rbp-0x35],sil
  401553:	8a 45 cb             	mov    al,BYTE PTR [rbp-0x35]
  401556:	a8 01                	test   al,0x1
  401558:	0f 85 05 00 00 00    	jne    401563 <decode_format+0x183>
  40155e:	e9 2c 00 00 00       	jmp    40158f <decode_format+0x1af>
	{
		width[0] = (width[0] * 10) + (buffer[buffer_index] - '0');
  401563:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  401567:	6b 08 0a             	imul   ecx,DWORD PTR [rax],0xa
  40156a:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  40156e:	48 63 55 cc          	movsxd rdx,DWORD PTR [rbp-0x34]
  401572:	0f be 34 10          	movsx  esi,BYTE PTR [rax+rdx*1]
  401576:	83 ee 30             	sub    esi,0x30
  401579:	01 f1                	add    ecx,esi
  40157b:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  40157f:	89 08                	mov    DWORD PTR [rax],ecx
		buffer_index++;
  401581:	8b 4d cc             	mov    ecx,DWORD PTR [rbp-0x34]
  401584:	83 c1 01             	add    ecx,0x1
  401587:	89 4d cc             	mov    DWORD PTR [rbp-0x34],ecx
	while (isdigit(buffer[buffer_index]))
  40158a:	e9 91 ff ff ff       	jmp    401520 <decode_format+0x140>
	}
	if (buffer[buffer_index] == '.')
  40158f:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  401593:	48 63 4d cc          	movsxd rcx,DWORD PTR [rbp-0x34]
  401597:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  40159b:	83 fa 2e             	cmp    edx,0x2e
  40159e:	0f 85 87 00 00 00    	jne    40162b <decode_format+0x24b>
	{
		precision[0] = 0;
  4015a4:	48 8b 45 e0          	mov    rax,QWORD PTR [rbp-0x20]
  4015a8:	c7 00 00 00 00 00    	mov    DWORD PTR [rax],0x0
		buffer_index++;
  4015ae:	8b 4d cc             	mov    ecx,DWORD PTR [rbp-0x34]
  4015b1:	83 c1 01             	add    ecx,0x1
  4015b4:	89 4d cc             	mov    DWORD PTR [rbp-0x34],ecx
  4015b7:	31 c0                	xor    eax,eax
  4015b9:	88 c1                	mov    cl,al
		while (isdigit(buffer[buffer_index]))
  4015bb:	48 8b 55 f8          	mov    rdx,QWORD PTR [rbp-0x8]
  4015bf:	48 63 75 cc          	movsxd rsi,DWORD PTR [rbp-0x34]
  4015c3:	0f be 04 32          	movsx  eax,BYTE PTR [rdx+rsi*1]
  4015c7:	83 f8 30             	cmp    eax,0x30
  4015ca:	88 4d ca             	mov    BYTE PTR [rbp-0x36],cl
  4015cd:	0f 8c 17 00 00 00    	jl     4015ea <decode_format+0x20a>
  4015d3:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4015d7:	48 63 4d cc          	movsxd rcx,DWORD PTR [rbp-0x34]
  4015db:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  4015df:	83 fa 39             	cmp    edx,0x39
  4015e2:	40 0f 9e c6          	setle  sil
  4015e6:	40 88 75 ca          	mov    BYTE PTR [rbp-0x36],sil
  4015ea:	8a 45 ca             	mov    al,BYTE PTR [rbp-0x36]
  4015ed:	a8 01                	test   al,0x1
  4015ef:	0f 85 05 00 00 00    	jne    4015fa <decode_format+0x21a>
  4015f5:	e9 2c 00 00 00       	jmp    401626 <decode_format+0x246>
		{
			precision[0] = (precision[0] * 10) + (buffer[buffer_index] - '0');
  4015fa:	48 8b 45 e0          	mov    rax,QWORD PTR [rbp-0x20]
  4015fe:	6b 08 0a             	imul   ecx,DWORD PTR [rax],0xa
  401601:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  401605:	48 63 55 cc          	movsxd rdx,DWORD PTR [rbp-0x34]
  401609:	0f be 34 10          	movsx  esi,BYTE PTR [rax+rdx*1]
  40160d:	83 ee 30             	sub    esi,0x30
  401610:	01 f1                	add    ecx,esi
  401612:	48 8b 45 e0          	mov    rax,QWORD PTR [rbp-0x20]
  401616:	89 08                	mov    DWORD PTR [rax],ecx
			buffer_index++;
  401618:	8b 4d cc             	mov    ecx,DWORD PTR [rbp-0x34]
  40161b:	83 c1 01             	add    ecx,0x1
  40161e:	89 4d cc             	mov    DWORD PTR [rbp-0x34],ecx
		while (isdigit(buffer[buffer_index]))
  401621:	e9 91 ff ff ff       	jmp    4015b7 <decode_format+0x1d7>
		}
	}
  401626:	e9 00 00 00 00       	jmp    40162b <decode_format+0x24b>
	if (buffer[buffer_index] == 'h')
  40162b:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  40162f:	48 63 4d cc          	movsxd rcx,DWORD PTR [rbp-0x34]
  401633:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  401637:	83 fa 68             	cmp    edx,0x68
  40163a:	0f 85 4a 00 00 00    	jne    40168a <decode_format+0x2aa>
	{
		if (buffer[buffer_index + 1] == 'h')
  401640:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  401644:	8b 4d cc             	mov    ecx,DWORD PTR [rbp-0x34]
  401647:	83 c1 01             	add    ecx,0x1
  40164a:	48 63 d1             	movsxd rdx,ecx
  40164d:	0f be 0c 10          	movsx  ecx,BYTE PTR [rax+rdx*1]
  401651:	83 f9 68             	cmp    ecx,0x68
  401654:	0f 85 18 00 00 00    	jne    401672 <decode_format+0x292>
		{
			length[0] = WidthFlags_hh;
  40165a:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  40165e:	c7 00 01 00 00 00    	mov    DWORD PTR [rax],0x1
			buffer_index++;
  401664:	8b 4d cc             	mov    ecx,DWORD PTR [rbp-0x34]
  401667:	83 c1 01             	add    ecx,0x1
  40166a:	89 4d cc             	mov    DWORD PTR [rbp-0x34],ecx
		}
  40166d:	e9 0a 00 00 00       	jmp    40167c <decode_format+0x29c>
		else
			length[0] = WidthFlags_h;
  401672:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  401676:	c7 00 02 00 00 00    	mov    DWORD PTR [rax],0x2
		buffer_index++;
  40167c:	8b 45 cc             	mov    eax,DWORD PTR [rbp-0x34]
  40167f:	83 c0 01             	add    eax,0x1
  401682:	89 45 cc             	mov    DWORD PTR [rbp-0x34],eax
	}
  401685:	e9 36 01 00 00       	jmp    4017c0 <decode_format+0x3e0>
	else if (buffer[buffer_index] == 'l')
  40168a:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  40168e:	48 63 4d cc          	movsxd rcx,DWORD PTR [rbp-0x34]
  401692:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  401696:	83 fa 6c             	cmp    edx,0x6c
  401699:	0f 85 4a 00 00 00    	jne    4016e9 <decode_format+0x309>
	{
		if (buffer[buffer_index + 1] == 'l')
  40169f:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4016a3:	8b 4d cc             	mov    ecx,DWORD PTR [rbp-0x34]
  4016a6:	83 c1 01             	add    ecx,0x1
  4016a9:	48 63 d1             	movsxd rdx,ecx
  4016ac:	0f be 0c 10          	movsx  ecx,BYTE PTR [rax+rdx*1]
  4016b0:	83 f9 6c             	cmp    ecx,0x6c
  4016b3:	0f 85 18 00 00 00    	jne    4016d1 <decode_format+0x2f1>
		{
			length[0] = WidthFlags_ll;
  4016b9:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  4016bd:	c7 00 04 00 00 00    	mov    DWORD PTR [rax],0x4
			buffer_index++;
  4016c3:	8b 4d cc             	mov    ecx,DWORD PTR [rbp-0x34]
  4016c6:	83 c1 01             	add    ecx,0x1
  4016c9:	89 4d cc             	mov    DWORD PTR [rbp-0x34],ecx
		}
  4016cc:	e9 0a 00 00 00       	jmp    4016db <decode_format+0x2fb>
		else
			length[0] = WidthFlags_l;
  4016d1:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  4016d5:	c7 00 03 00 00 00    	mov    DWORD PTR [rax],0x3
		buffer_index++;
  4016db:	8b 45 cc             	mov    eax,DWORD PTR [rbp-0x34]
  4016de:	83 c0 01             	add    eax,0x1
  4016e1:	89 45 cc             	mov    DWORD PTR [rbp-0x34],eax
	}
  4016e4:	e9 d2 00 00 00       	jmp    4017bb <decode_format+0x3db>
	else if (buffer[buffer_index] == 'j')
  4016e9:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4016ed:	48 63 4d cc          	movsxd rcx,DWORD PTR [rbp-0x34]
  4016f1:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  4016f5:	83 fa 6a             	cmp    edx,0x6a
  4016f8:	0f 85 18 00 00 00    	jne    401716 <decode_format+0x336>
	{
		length[0] = WidthFlags_j;
  4016fe:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  401702:	c7 00 05 00 00 00    	mov    DWORD PTR [rax],0x5
		buffer_index++;
  401708:	8b 4d cc             	mov    ecx,DWORD PTR [rbp-0x34]
  40170b:	83 c1 01             	add    ecx,0x1
  40170e:	89 4d cc             	mov    DWORD PTR [rbp-0x34],ecx
	}
  401711:	e9 a0 00 00 00       	jmp    4017b6 <decode_format+0x3d6>
	else if (buffer[buffer_index] == 'z')
  401716:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  40171a:	48 63 4d cc          	movsxd rcx,DWORD PTR [rbp-0x34]
  40171e:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  401722:	83 fa 7a             	cmp    edx,0x7a
  401725:	0f 85 18 00 00 00    	jne    401743 <decode_format+0x363>
	{
		length[0] = WidthFlags_z;
  40172b:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  40172f:	c7 00 06 00 00 00    	mov    DWORD PTR [rax],0x6
		buffer_index++;
  401735:	8b 4d cc             	mov    ecx,DWORD PTR [rbp-0x34]
  401738:	83 c1 01             	add    ecx,0x1
  40173b:	89 4d cc             	mov    DWORD PTR [rbp-0x34],ecx
	}
  40173e:	e9 6e 00 00 00       	jmp    4017b1 <decode_format+0x3d1>
	else if (buffer[buffer_index] == 't')
  401743:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  401747:	48 63 4d cc          	movsxd rcx,DWORD PTR [rbp-0x34]
  40174b:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  40174f:	83 fa 74             	cmp    edx,0x74
  401752:	0f 85 18 00 00 00    	jne    401770 <decode_format+0x390>
	{
		length[0] = WidthFlags_t;
  401758:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  40175c:	c7 00 07 00 00 00    	mov    DWORD PTR [rax],0x7
		buffer_index++;
  401762:	8b 4d cc             	mov    ecx,DWORD PTR [rbp-0x34]
  401765:	83 c1 01             	add    ecx,0x1
  401768:	89 4d cc             	mov    DWORD PTR [rbp-0x34],ecx
	}
  40176b:	e9 3c 00 00 00       	jmp    4017ac <decode_format+0x3cc>
	else if (buffer[buffer_index] == 'L')
  401770:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  401774:	48 63 4d cc          	movsxd rcx,DWORD PTR [rbp-0x34]
  401778:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  40177c:	83 fa 4c             	cmp    edx,0x4c
  40177f:	0f 85 18 00 00 00    	jne    40179d <decode_format+0x3bd>
	{
		length[0] = WidthFlags_L;
  401785:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  401789:	c7 00 08 00 00 00    	mov    DWORD PTR [rax],0x8
		buffer_index++;
  40178f:	8b 4d cc             	mov    ecx,DWORD PTR [rbp-0x34]
  401792:	83 c1 01             	add    ecx,0x1
  401795:	89 4d cc             	mov    DWORD PTR [rbp-0x34],ecx
	}
  401798:	e9 0a 00 00 00       	jmp    4017a7 <decode_format+0x3c7>
	else
	{
		length[0] = WidthFlags_None;
  40179d:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  4017a1:	c7 00 00 00 00 00    	mov    DWORD PTR [rax],0x0
  4017a7:	e9 00 00 00 00       	jmp    4017ac <decode_format+0x3cc>
  4017ac:	e9 00 00 00 00       	jmp    4017b1 <decode_format+0x3d1>
  4017b1:	e9 00 00 00 00       	jmp    4017b6 <decode_format+0x3d6>
  4017b6:	e9 00 00 00 00       	jmp    4017bb <decode_format+0x3db>
  4017bb:	e9 00 00 00 00       	jmp    4017c0 <decode_format+0x3e0>
	}
	specifier[0] = buffer[buffer_index];
  4017c0:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4017c4:	48 63 4d cc          	movsxd rcx,DWORD PTR [rbp-0x34]
  4017c8:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  4017cc:	48 8b 45 d0          	mov    rax,QWORD PTR [rbp-0x30]
  4017d0:	89 10                	mov    DWORD PTR [rax],edx
	buffer_index++;
  4017d2:	8b 55 cc             	mov    edx,DWORD PTR [rbp-0x34]
  4017d5:	83 c2 01             	add    edx,0x1
  4017d8:	89 55 cc             	mov    DWORD PTR [rbp-0x34],edx
	return buffer_index;
  4017db:	8b 45 cc             	mov    eax,DWORD PTR [rbp-0x34]
  4017de:	5d                   	pop    rbp
  4017df:	c3                   	ret    

00000000004017e0 <print_number>:
}
int print_number(int64_t number, int base, char *buffer, enum FormatFlag flags, int width, int precision, enum WidthFlags length, int specifier)
{
  4017e0:	55                   	push   rbp
  4017e1:	48 89 e5             	mov    rbp,rsp
  4017e4:	41 56                	push   r14
  4017e6:	53                   	push   rbx
  4017e7:	48 81 ec 80 01 00 00 	sub    rsp,0x180
  4017ee:	8b 45 18             	mov    eax,DWORD PTR [rbp+0x18]
  4017f1:	44 8b 55 10          	mov    r10d,DWORD PTR [rbp+0x10]
  4017f5:	45 31 db             	xor    r11d,r11d
  4017f8:	bb 00 01 00 00       	mov    ebx,0x100
  4017fd:	41 89 de             	mov    r14d,ebx
  401800:	48 89 7d e8          	mov    QWORD PTR [rbp-0x18],rdi
  401804:	89 75 e4             	mov    DWORD PTR [rbp-0x1c],esi
  401807:	48 89 55 d8          	mov    QWORD PTR [rbp-0x28],rdx
  40180b:	89 4d d4             	mov    DWORD PTR [rbp-0x2c],ecx
  40180e:	44 89 45 d0          	mov    DWORD PTR [rbp-0x30],r8d
  401812:	44 89 4d cc          	mov    DWORD PTR [rbp-0x34],r9d
	char symbols[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  401816:	48 8b 14 25 f0 26 40 	mov    rdx,QWORD PTR ds:0x4026f0
  40181d:	00 
  40181e:	48 89 55 b0          	mov    QWORD PTR [rbp-0x50],rdx
  401822:	48 8b 14 25 f8 26 40 	mov    rdx,QWORD PTR ds:0x4026f8
  401829:	00 
  40182a:	48 89 55 b8          	mov    QWORD PTR [rbp-0x48],rdx
	char digits[256] = {0};
  40182e:	48 8d 95 b0 fe ff ff 	lea    rdx,[rbp-0x150]
  401835:	48 89 d7             	mov    rdi,rdx
  401838:	44 89 de             	mov    esi,r11d
  40183b:	4c 89 f2             	mov    rdx,r14
  40183e:	44 89 95 88 fe ff ff 	mov    DWORD PTR [rbp-0x178],r10d
  401845:	89 85 84 fe ff ff    	mov    DWORD PTR [rbp-0x17c],eax
  40184b:	e8 00 e9 ff ff       	call   400150 <memset>
	int digits_index = 0, buffer_index = 0;
  401850:	c7 85 ac fe ff ff 00 	mov    DWORD PTR [rbp-0x154],0x0
  401857:	00 00 00 
  40185a:	c7 85 a8 fe ff ff 00 	mov    DWORD PTR [rbp-0x158],0x0
  401861:	00 00 00 
	int isNeg = 0;
  401864:	c7 85 a4 fe ff ff 00 	mov    DWORD PTR [rbp-0x15c],0x0
  40186b:	00 00 00 
	if (number < 0)
  40186e:	48 83 7d e8 00       	cmp    QWORD PTR [rbp-0x18],0x0
  401873:	0f 8d 13 00 00 00    	jge    40188c <print_number+0xac>
	{
		isNeg = 1;
  401879:	c7 85 a4 fe ff ff 01 	mov    DWORD PTR [rbp-0x15c],0x1
  401880:	00 00 00 
		number *= -1;
  401883:	48 6b 45 e8 ff       	imul   rax,QWORD PTR [rbp-0x18],0xffffffffffffffff
  401888:	48 89 45 e8          	mov    QWORD PTR [rbp-0x18],rax
	}
	if (flags == FormatFlag_PadZeros && precision != -1)
  40188c:	83 7d d4 05          	cmp    DWORD PTR [rbp-0x2c],0x5
  401890:	0f 85 11 00 00 00    	jne    4018a7 <print_number+0xc7>
  401896:	83 7d cc ff          	cmp    DWORD PTR [rbp-0x34],0xffffffff
  40189a:	0f 84 07 00 00 00    	je     4018a7 <print_number+0xc7>
	{
		flags = FormatFlag_None;
  4018a0:	c7 45 d4 00 00 00 00 	mov    DWORD PTR [rbp-0x2c],0x0
	}
	if (precision == -1)
  4018a7:	83 7d cc ff          	cmp    DWORD PTR [rbp-0x34],0xffffffff
  4018ab:	0f 85 07 00 00 00    	jne    4018b8 <print_number+0xd8>
		precision = 0;
  4018b1:	c7 45 cc 00 00 00 00 	mov    DWORD PTR [rbp-0x34],0x0
	do
  4018b8:	e9 00 00 00 00       	jmp    4018bd <print_number+0xdd>
	{
		digits[digits_index] = symbols[number % base];
  4018bd:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  4018c1:	48 63 4d e4          	movsxd rcx,DWORD PTR [rbp-0x1c]
  4018c5:	48 99                	cqo    
  4018c7:	48 f7 f9             	idiv   rcx
  4018ca:	40 8a 74 15 b0       	mov    sil,BYTE PTR [rbp+rdx*1-0x50]
  4018cf:	48 63 8d ac fe ff ff 	movsxd rcx,DWORD PTR [rbp-0x154]
  4018d6:	40 88 b4 0d b0 fe ff 	mov    BYTE PTR [rbp+rcx*1-0x150],sil
  4018dd:	ff 
		number /= base;
  4018de:	48 63 4d e4          	movsxd rcx,DWORD PTR [rbp-0x1c]
  4018e2:	48 8b 55 e8          	mov    rdx,QWORD PTR [rbp-0x18]
  4018e6:	48 89 d0             	mov    rax,rdx
  4018e9:	48 99                	cqo    
  4018eb:	48 f7 f9             	idiv   rcx
  4018ee:	48 89 45 e8          	mov    QWORD PTR [rbp-0x18],rax
		digits_index++;
  4018f2:	8b bd ac fe ff ff    	mov    edi,DWORD PTR [rbp-0x154]
  4018f8:	83 c7 01             	add    edi,0x1
  4018fb:	89 bd ac fe ff ff    	mov    DWORD PTR [rbp-0x154],edi
	} while (number > 0);
  401901:	48 83 7d e8 00       	cmp    QWORD PTR [rbp-0x18],0x0
  401906:	0f 8f b1 ff ff ff    	jg     4018bd <print_number+0xdd>
	//  add leading zeros
	precision -= digits_index;
  40190c:	8b 85 ac fe ff ff    	mov    eax,DWORD PTR [rbp-0x154]
  401912:	8b 4d cc             	mov    ecx,DWORD PTR [rbp-0x34]
  401915:	29 c1                	sub    ecx,eax
  401917:	89 4d cc             	mov    DWORD PTR [rbp-0x34],ecx
	for (int i = 0; i < precision; i++)
  40191a:	c7 85 a0 fe ff ff 00 	mov    DWORD PTR [rbp-0x160],0x0
  401921:	00 00 00 
  401924:	8b 85 a0 fe ff ff    	mov    eax,DWORD PTR [rbp-0x160]
  40192a:	3b 45 cc             	cmp    eax,DWORD PTR [rbp-0x34]
  40192d:	0f 8d 32 00 00 00    	jge    401965 <print_number+0x185>
	{
		digits[digits_index++] = symbols[0];
  401933:	8a 45 b0             	mov    al,BYTE PTR [rbp-0x50]
  401936:	8b 8d ac fe ff ff    	mov    ecx,DWORD PTR [rbp-0x154]
  40193c:	89 ca                	mov    edx,ecx
  40193e:	83 c2 01             	add    edx,0x1
  401941:	89 95 ac fe ff ff    	mov    DWORD PTR [rbp-0x154],edx
  401947:	48 63 f1             	movsxd rsi,ecx
  40194a:	88 84 35 b0 fe ff ff 	mov    BYTE PTR [rbp+rsi*1-0x150],al
	for (int i = 0; i < precision; i++)
  401951:	8b 85 a0 fe ff ff    	mov    eax,DWORD PTR [rbp-0x160]
  401957:	83 c0 01             	add    eax,0x1
  40195a:	89 85 a0 fe ff ff    	mov    DWORD PTR [rbp-0x160],eax
  401960:	e9 bf ff ff ff       	jmp    401924 <print_number+0x144>
	}
	int conditional_space = 0;
  401965:	c7 85 9c fe ff ff 00 	mov    DWORD PTR [rbp-0x164],0x0
  40196c:	00 00 00 
	if (isNeg || (specifier != 'x' && flags == FormatFlag_BlankIfNoSign) || (specifier != 'x' && flags == FormatFlag_PrependSign))
  40196f:	83 bd a4 fe ff ff 00 	cmp    DWORD PTR [rbp-0x15c],0x0
  401976:	0f 85 28 00 00 00    	jne    4019a4 <print_number+0x1c4>
  40197c:	83 7d 18 78          	cmp    DWORD PTR [rbp+0x18],0x78
  401980:	0f 84 0a 00 00 00    	je     401990 <print_number+0x1b0>
  401986:	83 7d d4 03          	cmp    DWORD PTR [rbp-0x2c],0x3
  40198a:	0f 84 14 00 00 00    	je     4019a4 <print_number+0x1c4>
  401990:	83 7d 18 78          	cmp    DWORD PTR [rbp+0x18],0x78
  401994:	0f 84 14 00 00 00    	je     4019ae <print_number+0x1ce>
  40199a:	83 7d d4 02          	cmp    DWORD PTR [rbp-0x2c],0x2
  40199e:	0f 85 0a 00 00 00    	jne    4019ae <print_number+0x1ce>
	{
		conditional_space = 1;
  4019a4:	c7 85 9c fe ff ff 01 	mov    DWORD PTR [rbp-0x164],0x1
  4019ab:	00 00 00 
	}
	if (specifier == 'x' && flags == FormatFlag_PrependBaseNotifier)
  4019ae:	83 7d 18 78          	cmp    DWORD PTR [rbp+0x18],0x78
  4019b2:	0f 85 14 00 00 00    	jne    4019cc <print_number+0x1ec>
  4019b8:	83 7d d4 04          	cmp    DWORD PTR [rbp-0x2c],0x4
  4019bc:	0f 85 0a 00 00 00    	jne    4019cc <print_number+0x1ec>
	{
		conditional_space = 2;
  4019c2:	c7 85 9c fe ff ff 02 	mov    DWORD PTR [rbp-0x164],0x2
  4019c9:	00 00 00 
	}

	if ((flags != FormatFlag_PadZeros) && flags != FormatFlag_LeftJustify)
  4019cc:	83 7d d4 05          	cmp    DWORD PTR [rbp-0x2c],0x5
  4019d0:	0f 84 7b 00 00 00    	je     401a51 <print_number+0x271>
  4019d6:	83 7d d4 01          	cmp    DWORD PTR [rbp-0x2c],0x1
  4019da:	0f 84 71 00 00 00    	je     401a51 <print_number+0x271>
		for (int i = 0; i < max(0, width - digits_index - conditional_space); i++)
  4019e0:	c7 85 98 fe ff ff 00 	mov    DWORD PTR [rbp-0x168],0x0
  4019e7:	00 00 00 
  4019ea:	31 ff                	xor    edi,edi
  4019ec:	8b 85 98 fe ff ff    	mov    eax,DWORD PTR [rbp-0x168]
  4019f2:	8b 4d d0             	mov    ecx,DWORD PTR [rbp-0x30]
  4019f5:	2b 8d ac fe ff ff    	sub    ecx,DWORD PTR [rbp-0x154]
  4019fb:	2b 8d 9c fe ff ff    	sub    ecx,DWORD PTR [rbp-0x164]
  401a01:	89 ce                	mov    esi,ecx
  401a03:	89 85 80 fe ff ff    	mov    DWORD PTR [rbp-0x180],eax
  401a09:	e8 a2 f9 ff ff       	call   4013b0 <max>
  401a0e:	8b 8d 80 fe ff ff    	mov    ecx,DWORD PTR [rbp-0x180]
  401a14:	39 c1                	cmp    ecx,eax
  401a16:	0f 8d 30 00 00 00    	jge    401a4c <print_number+0x26c>
			buffer[buffer_index++] = ' ';
  401a1c:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  401a20:	8b 8d a8 fe ff ff    	mov    ecx,DWORD PTR [rbp-0x158]
  401a26:	89 ca                	mov    edx,ecx
  401a28:	83 c2 01             	add    edx,0x1
  401a2b:	89 95 a8 fe ff ff    	mov    DWORD PTR [rbp-0x158],edx
  401a31:	48 63 f1             	movsxd rsi,ecx
  401a34:	c6 04 30 20          	mov    BYTE PTR [rax+rsi*1],0x20
		for (int i = 0; i < max(0, width - digits_index - conditional_space); i++)
  401a38:	8b 85 98 fe ff ff    	mov    eax,DWORD PTR [rbp-0x168]
  401a3e:	83 c0 01             	add    eax,0x1
  401a41:	89 85 98 fe ff ff    	mov    DWORD PTR [rbp-0x168],eax
  401a47:	e9 9e ff ff ff       	jmp    4019ea <print_number+0x20a>
			buffer[buffer_index++] = ' ';
  401a4c:	e9 00 00 00 00       	jmp    401a51 <print_number+0x271>
	if (isNeg)
  401a51:	83 bd a4 fe ff ff 00 	cmp    DWORD PTR [rbp-0x15c],0x0
  401a58:	0f 84 23 00 00 00    	je     401a81 <print_number+0x2a1>
	{
		buffer[buffer_index] = '-';
  401a5e:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  401a62:	48 63 8d a8 fe ff ff 	movsxd rcx,DWORD PTR [rbp-0x158]
  401a69:	c6 04 08 2d          	mov    BYTE PTR [rax+rcx*1],0x2d
		buffer_index++;
  401a6d:	8b 95 a8 fe ff ff    	mov    edx,DWORD PTR [rbp-0x158]
  401a73:	83 c2 01             	add    edx,0x1
  401a76:	89 95 a8 fe ff ff    	mov    DWORD PTR [rbp-0x158],edx
	}
  401a7c:	e9 c9 00 00 00       	jmp    401b4a <print_number+0x36a>
	else if (flags == FormatFlag_PrependSign && specifier != 'x')
  401a81:	83 7d d4 02          	cmp    DWORD PTR [rbp-0x2c],0x2
  401a85:	0f 85 2d 00 00 00    	jne    401ab8 <print_number+0x2d8>
  401a8b:	83 7d 18 78          	cmp    DWORD PTR [rbp+0x18],0x78
  401a8f:	0f 84 23 00 00 00    	je     401ab8 <print_number+0x2d8>
	{
		buffer[buffer_index] = '+';
  401a95:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  401a99:	48 63 8d a8 fe ff ff 	movsxd rcx,DWORD PTR [rbp-0x158]
  401aa0:	c6 04 08 2b          	mov    BYTE PTR [rax+rcx*1],0x2b
		buffer_index++;
  401aa4:	8b 95 a8 fe ff ff    	mov    edx,DWORD PTR [rbp-0x158]
  401aaa:	83 c2 01             	add    edx,0x1
  401aad:	89 95 a8 fe ff ff    	mov    DWORD PTR [rbp-0x158],edx
	}
  401ab3:	e9 8d 00 00 00       	jmp    401b45 <print_number+0x365>
	else if (flags == FormatFlag_BlankIfNoSign && specifier != 'x')
  401ab8:	83 7d d4 03          	cmp    DWORD PTR [rbp-0x2c],0x3
  401abc:	0f 85 2d 00 00 00    	jne    401aef <print_number+0x30f>
  401ac2:	83 7d 18 78          	cmp    DWORD PTR [rbp+0x18],0x78
  401ac6:	0f 84 23 00 00 00    	je     401aef <print_number+0x30f>
	{
		buffer[buffer_index] = ' ';
  401acc:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  401ad0:	48 63 8d a8 fe ff ff 	movsxd rcx,DWORD PTR [rbp-0x158]
  401ad7:	c6 04 08 20          	mov    BYTE PTR [rax+rcx*1],0x20
		buffer_index++;
  401adb:	8b 95 a8 fe ff ff    	mov    edx,DWORD PTR [rbp-0x158]
  401ae1:	83 c2 01             	add    edx,0x1
  401ae4:	89 95 a8 fe ff ff    	mov    DWORD PTR [rbp-0x158],edx
	}
  401aea:	e9 51 00 00 00       	jmp    401b40 <print_number+0x360>
	else if (specifier == 'x' && flags == FormatFlag_PrependBaseNotifier)
  401aef:	83 7d 18 78          	cmp    DWORD PTR [rbp+0x18],0x78
  401af3:	0f 85 42 00 00 00    	jne    401b3b <print_number+0x35b>
  401af9:	83 7d d4 04          	cmp    DWORD PTR [rbp-0x2c],0x4
  401afd:	0f 85 38 00 00 00    	jne    401b3b <print_number+0x35b>
	{
		buffer[buffer_index++] = '0';
  401b03:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  401b07:	8b 8d a8 fe ff ff    	mov    ecx,DWORD PTR [rbp-0x158]
  401b0d:	89 ca                	mov    edx,ecx
  401b0f:	83 c2 01             	add    edx,0x1
  401b12:	89 95 a8 fe ff ff    	mov    DWORD PTR [rbp-0x158],edx
  401b18:	48 63 f1             	movsxd rsi,ecx
  401b1b:	c6 04 30 30          	mov    BYTE PTR [rax+rsi*1],0x30
		buffer[buffer_index++] = 'x';
  401b1f:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  401b23:	8b 8d a8 fe ff ff    	mov    ecx,DWORD PTR [rbp-0x158]
  401b29:	89 ca                	mov    edx,ecx
  401b2b:	83 c2 01             	add    edx,0x1
  401b2e:	89 95 a8 fe ff ff    	mov    DWORD PTR [rbp-0x158],edx
  401b34:	48 63 f1             	movsxd rsi,ecx
  401b37:	c6 04 30 78          	mov    BYTE PTR [rax+rsi*1],0x78
  401b3b:	e9 00 00 00 00       	jmp    401b40 <print_number+0x360>
  401b40:	e9 00 00 00 00       	jmp    401b45 <print_number+0x365>
  401b45:	e9 00 00 00 00       	jmp    401b4a <print_number+0x36a>
	}
	if (flags == FormatFlag_PadZeros && precision == 0)
  401b4a:	83 7d d4 05          	cmp    DWORD PTR [rbp-0x2c],0x5
  401b4e:	0f 85 7b 00 00 00    	jne    401bcf <print_number+0x3ef>
  401b54:	83 7d cc 00          	cmp    DWORD PTR [rbp-0x34],0x0
  401b58:	0f 85 71 00 00 00    	jne    401bcf <print_number+0x3ef>
		for (int i = 0; i < max(0, width - digits_index - conditional_space); i++)
  401b5e:	c7 85 94 fe ff ff 00 	mov    DWORD PTR [rbp-0x16c],0x0
  401b65:	00 00 00 
  401b68:	31 ff                	xor    edi,edi
  401b6a:	8b 85 94 fe ff ff    	mov    eax,DWORD PTR [rbp-0x16c]
  401b70:	8b 4d d0             	mov    ecx,DWORD PTR [rbp-0x30]
  401b73:	2b 8d ac fe ff ff    	sub    ecx,DWORD PTR [rbp-0x154]
  401b79:	2b 8d 9c fe ff ff    	sub    ecx,DWORD PTR [rbp-0x164]
  401b7f:	89 ce                	mov    esi,ecx
  401b81:	89 85 7c fe ff ff    	mov    DWORD PTR [rbp-0x184],eax
  401b87:	e8 24 f8 ff ff       	call   4013b0 <max>
  401b8c:	8b 8d 7c fe ff ff    	mov    ecx,DWORD PTR [rbp-0x184]
  401b92:	39 c1                	cmp    ecx,eax
  401b94:	0f 8d 30 00 00 00    	jge    401bca <print_number+0x3ea>
			buffer[buffer_index++] = '0';
  401b9a:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  401b9e:	8b 8d a8 fe ff ff    	mov    ecx,DWORD PTR [rbp-0x158]
  401ba4:	89 ca                	mov    edx,ecx
  401ba6:	83 c2 01             	add    edx,0x1
  401ba9:	89 95 a8 fe ff ff    	mov    DWORD PTR [rbp-0x158],edx
  401baf:	48 63 f1             	movsxd rsi,ecx
  401bb2:	c6 04 30 30          	mov    BYTE PTR [rax+rsi*1],0x30
		for (int i = 0; i < max(0, width - digits_index - conditional_space); i++)
  401bb6:	8b 85 94 fe ff ff    	mov    eax,DWORD PTR [rbp-0x16c]
  401bbc:	83 c0 01             	add    eax,0x1
  401bbf:	89 85 94 fe ff ff    	mov    DWORD PTR [rbp-0x16c],eax
  401bc5:	e9 9e ff ff ff       	jmp    401b68 <print_number+0x388>
			buffer[buffer_index++] = '0';
  401bca:	e9 00 00 00 00       	jmp    401bcf <print_number+0x3ef>
	for (int i = digits_index - 1; i >= 0; i--)
  401bcf:	8b 85 ac fe ff ff    	mov    eax,DWORD PTR [rbp-0x154]
  401bd5:	83 e8 01             	sub    eax,0x1
  401bd8:	89 85 90 fe ff ff    	mov    DWORD PTR [rbp-0x170],eax
  401bde:	83 bd 90 fe ff ff 00 	cmp    DWORD PTR [rbp-0x170],0x0
  401be5:	0f 8c 3f 00 00 00    	jl     401c2a <print_number+0x44a>
	{
		buffer[buffer_index] = digits[i];
  401beb:	48 63 85 90 fe ff ff 	movsxd rax,DWORD PTR [rbp-0x170]
  401bf2:	8a 8c 05 b0 fe ff ff 	mov    cl,BYTE PTR [rbp+rax*1-0x150]
  401bf9:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  401bfd:	48 63 95 a8 fe ff ff 	movsxd rdx,DWORD PTR [rbp-0x158]
  401c04:	88 0c 10             	mov    BYTE PTR [rax+rdx*1],cl
		buffer_index++;
  401c07:	8b b5 a8 fe ff ff    	mov    esi,DWORD PTR [rbp-0x158]
  401c0d:	83 c6 01             	add    esi,0x1
  401c10:	89 b5 a8 fe ff ff    	mov    DWORD PTR [rbp-0x158],esi
	for (int i = digits_index - 1; i >= 0; i--)
  401c16:	8b 85 90 fe ff ff    	mov    eax,DWORD PTR [rbp-0x170]
  401c1c:	83 c0 ff             	add    eax,0xffffffff
  401c1f:	89 85 90 fe ff ff    	mov    DWORD PTR [rbp-0x170],eax
  401c25:	e9 b4 ff ff ff       	jmp    401bde <print_number+0x3fe>
	}
	if (flags == FormatFlag_LeftJustify)
  401c2a:	83 7d d4 01          	cmp    DWORD PTR [rbp-0x2c],0x1
  401c2e:	0f 85 71 00 00 00    	jne    401ca5 <print_number+0x4c5>
		for (int i = 0; i < max(0, width - digits_index - conditional_space); i++)
  401c34:	c7 85 8c fe ff ff 00 	mov    DWORD PTR [rbp-0x174],0x0
  401c3b:	00 00 00 
  401c3e:	31 ff                	xor    edi,edi
  401c40:	8b 85 8c fe ff ff    	mov    eax,DWORD PTR [rbp-0x174]
  401c46:	8b 4d d0             	mov    ecx,DWORD PTR [rbp-0x30]
  401c49:	2b 8d ac fe ff ff    	sub    ecx,DWORD PTR [rbp-0x154]
  401c4f:	2b 8d 9c fe ff ff    	sub    ecx,DWORD PTR [rbp-0x164]
  401c55:	89 ce                	mov    esi,ecx
  401c57:	89 85 78 fe ff ff    	mov    DWORD PTR [rbp-0x188],eax
  401c5d:	e8 4e f7 ff ff       	call   4013b0 <max>
  401c62:	8b 8d 78 fe ff ff    	mov    ecx,DWORD PTR [rbp-0x188]
  401c68:	39 c1                	cmp    ecx,eax
  401c6a:	0f 8d 30 00 00 00    	jge    401ca0 <print_number+0x4c0>
			buffer[buffer_index++] = ' ';
  401c70:	48 8b 45 d8          	mov    rax,QWORD PTR [rbp-0x28]
  401c74:	8b 8d a8 fe ff ff    	mov    ecx,DWORD PTR [rbp-0x158]
  401c7a:	89 ca                	mov    edx,ecx
  401c7c:	83 c2 01             	add    edx,0x1
  401c7f:	89 95 a8 fe ff ff    	mov    DWORD PTR [rbp-0x158],edx
  401c85:	48 63 f1             	movsxd rsi,ecx
  401c88:	c6 04 30 20          	mov    BYTE PTR [rax+rsi*1],0x20
		for (int i = 0; i < max(0, width - digits_index - conditional_space); i++)
  401c8c:	8b 85 8c fe ff ff    	mov    eax,DWORD PTR [rbp-0x174]
  401c92:	83 c0 01             	add    eax,0x1
  401c95:	89 85 8c fe ff ff    	mov    DWORD PTR [rbp-0x174],eax
  401c9b:	e9 9e ff ff ff       	jmp    401c3e <print_number+0x45e>
			buffer[buffer_index++] = ' ';
  401ca0:	e9 00 00 00 00       	jmp    401ca5 <print_number+0x4c5>
	return buffer_index;
  401ca5:	8b 85 a8 fe ff ff    	mov    eax,DWORD PTR [rbp-0x158]
  401cab:	48 81 c4 80 01 00 00 	add    rsp,0x180
  401cb2:	5b                   	pop    rbx
  401cb3:	41 5e                	pop    r14
  401cb5:	5d                   	pop    rbp
  401cb6:	c3                   	ret    
  401cb7:	66 0f 1f 84 00 00 00 	nop    WORD PTR [rax+rax*1+0x0]
  401cbe:	00 00 

0000000000401cc0 <vsprintf>:
}
int vsprintf(char *buffer, const char *format, va_list arg)
{
  401cc0:	55                   	push   rbp
  401cc1:	48 89 e5             	mov    rbp,rsp
  401cc4:	48 81 ec 70 01 00 00 	sub    rsp,0x170
  401ccb:	48 89 7d f8          	mov    QWORD PTR [rbp-0x8],rdi
  401ccf:	48 89 75 f0          	mov    QWORD PTR [rbp-0x10],rsi
  401cd3:	48 89 55 e8          	mov    QWORD PTR [rbp-0x18],rdx
	int format_index = 0, buffer_index = 0;
  401cd7:	c7 45 e4 00 00 00 00 	mov    DWORD PTR [rbp-0x1c],0x0
  401cde:	c7 45 e0 00 00 00 00 	mov    DWORD PTR [rbp-0x20],0x0
	enum FormatFlag flags;
	enum WidthFlags length;
	int width, precision, specifier;
	while (format[format_index] != 0)
  401ce5:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  401ce9:	48 63 4d e4          	movsxd rcx,DWORD PTR [rbp-0x1c]
  401ced:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  401cf1:	83 fa 00             	cmp    edx,0x0
  401cf4:	0f 84 95 06 00 00    	je     40238f <vsprintf+0x6cf>
	{
		if (format[format_index] == '%')
  401cfa:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  401cfe:	48 63 4d e4          	movsxd rcx,DWORD PTR [rbp-0x1c]
  401d02:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  401d06:	83 fa 25             	cmp    edx,0x25
  401d09:	0f 85 53 06 00 00    	jne    402362 <vsprintf+0x6a2>
		{
			format_index += decode_format(format, &flags, &width, &precision, &length, &specifier);
  401d0f:	48 8b 7d f0          	mov    rdi,QWORD PTR [rbp-0x10]
  401d13:	48 8d 75 dc          	lea    rsi,[rbp-0x24]
  401d17:	48 8d 55 d4          	lea    rdx,[rbp-0x2c]
  401d1b:	48 8d 4d d0          	lea    rcx,[rbp-0x30]
  401d1f:	4c 8d 45 d8          	lea    r8,[rbp-0x28]
  401d23:	4c 8d 4d cc          	lea    r9,[rbp-0x34]
  401d27:	e8 b4 f6 ff ff       	call   4013e0 <decode_format>
  401d2c:	03 45 e4             	add    eax,DWORD PTR [rbp-0x1c]
  401d2f:	89 45 e4             	mov    DWORD PTR [rbp-0x1c],eax
			if (specifier == 'd' || specifier == 'i')
  401d32:	83 7d cc 64          	cmp    DWORD PTR [rbp-0x34],0x64
  401d36:	0f 84 0a 00 00 00    	je     401d46 <vsprintf+0x86>
  401d3c:	83 7d cc 69          	cmp    DWORD PTR [rbp-0x34],0x69
  401d40:	0f 85 3c 02 00 00    	jne    401f82 <vsprintf+0x2c2>
			{
				if (length == WidthFlags_None)
  401d46:	83 7d d8 00          	cmp    DWORD PTR [rbp-0x28],0x0
  401d4a:	0f 85 aa 00 00 00    	jne    401dfa <vsprintf+0x13a>
				{
					int number = va_arg(arg, int);
  401d50:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  401d54:	8b 08                	mov    ecx,DWORD PTR [rax]
  401d56:	83 f9 28             	cmp    ecx,0x28
  401d59:	48 89 45 80          	mov    QWORD PTR [rbp-0x80],rax
  401d5d:	89 8d 7c ff ff ff    	mov    DWORD PTR [rbp-0x84],ecx
  401d63:	0f 87 22 00 00 00    	ja     401d8b <vsprintf+0xcb>
  401d69:	8b 85 7c ff ff ff    	mov    eax,DWORD PTR [rbp-0x84]
  401d6f:	48 63 c8             	movsxd rcx,eax
  401d72:	48 8b 55 80          	mov    rdx,QWORD PTR [rbp-0x80]
  401d76:	48 03 4a 10          	add    rcx,QWORD PTR [rdx+0x10]
  401d7a:	83 c0 08             	add    eax,0x8
  401d7d:	89 02                	mov    DWORD PTR [rdx],eax
  401d7f:	48 89 8d 70 ff ff ff 	mov    QWORD PTR [rbp-0x90],rcx
  401d86:	e9 1a 00 00 00       	jmp    401da5 <vsprintf+0xe5>
  401d8b:	48 8b 45 80          	mov    rax,QWORD PTR [rbp-0x80]
  401d8f:	48 8b 48 08          	mov    rcx,QWORD PTR [rax+0x8]
  401d93:	48 89 ca             	mov    rdx,rcx
  401d96:	48 83 c1 08          	add    rcx,0x8
  401d9a:	48 89 48 08          	mov    QWORD PTR [rax+0x8],rcx
  401d9e:	48 89 95 70 ff ff ff 	mov    QWORD PTR [rbp-0x90],rdx
  401da5:	48 8b 85 70 ff ff ff 	mov    rax,QWORD PTR [rbp-0x90]
  401dac:	8b 08                	mov    ecx,DWORD PTR [rax]
  401dae:	89 4d c8             	mov    DWORD PTR [rbp-0x38],ecx
					buffer_index += print_number(number, 10, buffer, flags, width, precision, length, specifier);
  401db1:	48 63 7d c8          	movsxd rdi,DWORD PTR [rbp-0x38]
  401db5:	48 8b 55 f8          	mov    rdx,QWORD PTR [rbp-0x8]
  401db9:	8b 4d dc             	mov    ecx,DWORD PTR [rbp-0x24]
  401dbc:	44 8b 45 d4          	mov    r8d,DWORD PTR [rbp-0x2c]
  401dc0:	44 8b 4d d0          	mov    r9d,DWORD PTR [rbp-0x30]
  401dc4:	8b 75 d8             	mov    esi,DWORD PTR [rbp-0x28]
  401dc7:	44 8b 55 cc          	mov    r10d,DWORD PTR [rbp-0x34]
  401dcb:	41 bb 0a 00 00 00    	mov    r11d,0xa
  401dd1:	89 b5 6c ff ff ff    	mov    DWORD PTR [rbp-0x94],esi
  401dd7:	44 89 de             	mov    esi,r11d
  401dda:	44 8b 9d 6c ff ff ff 	mov    r11d,DWORD PTR [rbp-0x94]
  401de1:	44 89 1c 24          	mov    DWORD PTR [rsp],r11d
  401de5:	44 89 54 24 08       	mov    DWORD PTR [rsp+0x8],r10d
  401dea:	e8 f1 f9 ff ff       	call   4017e0 <print_number>
  401def:	03 45 e0             	add    eax,DWORD PTR [rbp-0x20]
  401df2:	89 45 e0             	mov    DWORD PTR [rbp-0x20],eax
				}
  401df5:	e9 83 01 00 00       	jmp    401f7d <vsprintf+0x2bd>
				else if (length == WidthFlags_l)
  401dfa:	83 7d d8 03          	cmp    DWORD PTR [rbp-0x28],0x3
  401dfe:	0f 85 b5 00 00 00    	jne    401eb9 <vsprintf+0x1f9>
				{
					long int number = va_arg(arg, long int);
  401e04:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  401e08:	8b 08                	mov    ecx,DWORD PTR [rax]
  401e0a:	83 f9 28             	cmp    ecx,0x28
  401e0d:	48 89 85 60 ff ff ff 	mov    QWORD PTR [rbp-0xa0],rax
  401e14:	89 8d 5c ff ff ff    	mov    DWORD PTR [rbp-0xa4],ecx
  401e1a:	0f 87 25 00 00 00    	ja     401e45 <vsprintf+0x185>
  401e20:	8b 85 5c ff ff ff    	mov    eax,DWORD PTR [rbp-0xa4]
  401e26:	48 63 c8             	movsxd rcx,eax
  401e29:	48 8b 95 60 ff ff ff 	mov    rdx,QWORD PTR [rbp-0xa0]
  401e30:	48 03 4a 10          	add    rcx,QWORD PTR [rdx+0x10]
  401e34:	83 c0 08             	add    eax,0x8
  401e37:	89 02                	mov    DWORD PTR [rdx],eax
  401e39:	48 89 8d 50 ff ff ff 	mov    QWORD PTR [rbp-0xb0],rcx
  401e40:	e9 1d 00 00 00       	jmp    401e62 <vsprintf+0x1a2>
  401e45:	48 8b 85 60 ff ff ff 	mov    rax,QWORD PTR [rbp-0xa0]
  401e4c:	48 8b 48 08          	mov    rcx,QWORD PTR [rax+0x8]
  401e50:	48 89 ca             	mov    rdx,rcx
  401e53:	48 83 c1 08          	add    rcx,0x8
  401e57:	48 89 48 08          	mov    QWORD PTR [rax+0x8],rcx
  401e5b:	48 89 95 50 ff ff ff 	mov    QWORD PTR [rbp-0xb0],rdx
  401e62:	48 8b 85 50 ff ff ff 	mov    rax,QWORD PTR [rbp-0xb0]
  401e69:	48 8b 00             	mov    rax,QWORD PTR [rax]
  401e6c:	48 89 45 c0          	mov    QWORD PTR [rbp-0x40],rax
					buffer_index += print_number(number, 10, buffer, flags, width, precision, length, specifier);
  401e70:	48 8b 7d c0          	mov    rdi,QWORD PTR [rbp-0x40]
  401e74:	48 8b 55 f8          	mov    rdx,QWORD PTR [rbp-0x8]
  401e78:	8b 4d dc             	mov    ecx,DWORD PTR [rbp-0x24]
  401e7b:	44 8b 45 d4          	mov    r8d,DWORD PTR [rbp-0x2c]
  401e7f:	44 8b 4d d0          	mov    r9d,DWORD PTR [rbp-0x30]
  401e83:	8b 75 d8             	mov    esi,DWORD PTR [rbp-0x28]
  401e86:	44 8b 55 cc          	mov    r10d,DWORD PTR [rbp-0x34]
  401e8a:	41 bb 0a 00 00 00    	mov    r11d,0xa
  401e90:	89 b5 4c ff ff ff    	mov    DWORD PTR [rbp-0xb4],esi
  401e96:	44 89 de             	mov    esi,r11d
  401e99:	44 8b 9d 4c ff ff ff 	mov    r11d,DWORD PTR [rbp-0xb4]
  401ea0:	44 89 1c 24          	mov    DWORD PTR [rsp],r11d
  401ea4:	44 89 54 24 08       	mov    DWORD PTR [rsp+0x8],r10d
  401ea9:	e8 32 f9 ff ff       	call   4017e0 <print_number>
  401eae:	03 45 e0             	add    eax,DWORD PTR [rbp-0x20]
  401eb1:	89 45 e0             	mov    DWORD PTR [rbp-0x20],eax
				}
  401eb4:	e9 bf 00 00 00       	jmp    401f78 <vsprintf+0x2b8>
				else if (length == WidthFlags_ll)
  401eb9:	83 7d d8 04          	cmp    DWORD PTR [rbp-0x28],0x4
  401ebd:	0f 85 b0 00 00 00    	jne    401f73 <vsprintf+0x2b3>
				{
					long long int number = va_arg(arg, long long int);
  401ec3:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  401ec7:	8b 08                	mov    ecx,DWORD PTR [rax]
  401ec9:	83 f9 28             	cmp    ecx,0x28
  401ecc:	48 89 85 40 ff ff ff 	mov    QWORD PTR [rbp-0xc0],rax
  401ed3:	89 8d 3c ff ff ff    	mov    DWORD PTR [rbp-0xc4],ecx
  401ed9:	0f 87 25 00 00 00    	ja     401f04 <vsprintf+0x244>
  401edf:	8b 85 3c ff ff ff    	mov    eax,DWORD PTR [rbp-0xc4]
  401ee5:	48 63 c8             	movsxd rcx,eax
  401ee8:	48 8b 95 40 ff ff ff 	mov    rdx,QWORD PTR [rbp-0xc0]
  401eef:	48 03 4a 10          	add    rcx,QWORD PTR [rdx+0x10]
  401ef3:	83 c0 08             	add    eax,0x8
  401ef6:	89 02                	mov    DWORD PTR [rdx],eax
  401ef8:	48 89 8d 30 ff ff ff 	mov    QWORD PTR [rbp-0xd0],rcx
  401eff:	e9 1d 00 00 00       	jmp    401f21 <vsprintf+0x261>
  401f04:	48 8b 85 40 ff ff ff 	mov    rax,QWORD PTR [rbp-0xc0]
  401f0b:	48 8b 48 08          	mov    rcx,QWORD PTR [rax+0x8]
  401f0f:	48 89 ca             	mov    rdx,rcx
  401f12:	48 83 c1 08          	add    rcx,0x8
  401f16:	48 89 48 08          	mov    QWORD PTR [rax+0x8],rcx
  401f1a:	48 89 95 30 ff ff ff 	mov    QWORD PTR [rbp-0xd0],rdx
  401f21:	48 8b 85 30 ff ff ff 	mov    rax,QWORD PTR [rbp-0xd0]
  401f28:	48 8b 00             	mov    rax,QWORD PTR [rax]
  401f2b:	48 89 45 b8          	mov    QWORD PTR [rbp-0x48],rax
					buffer_index += print_number(number, 10, buffer, flags, width, precision, length, specifier);
  401f2f:	48 8b 7d b8          	mov    rdi,QWORD PTR [rbp-0x48]
  401f33:	48 8b 55 f8          	mov    rdx,QWORD PTR [rbp-0x8]
  401f37:	8b 4d dc             	mov    ecx,DWORD PTR [rbp-0x24]
  401f3a:	44 8b 45 d4          	mov    r8d,DWORD PTR [rbp-0x2c]
  401f3e:	44 8b 4d d0          	mov    r9d,DWORD PTR [rbp-0x30]
  401f42:	8b 75 d8             	mov    esi,DWORD PTR [rbp-0x28]
  401f45:	44 8b 55 cc          	mov    r10d,DWORD PTR [rbp-0x34]
  401f49:	41 bb 0a 00 00 00    	mov    r11d,0xa
  401f4f:	89 b5 2c ff ff ff    	mov    DWORD PTR [rbp-0xd4],esi
  401f55:	44 89 de             	mov    esi,r11d
  401f58:	44 8b 9d 2c ff ff ff 	mov    r11d,DWORD PTR [rbp-0xd4]
  401f5f:	44 89 1c 24          	mov    DWORD PTR [rsp],r11d
  401f63:	44 89 54 24 08       	mov    DWORD PTR [rsp+0x8],r10d
  401f68:	e8 73 f8 ff ff       	call   4017e0 <print_number>
  401f6d:	03 45 e0             	add    eax,DWORD PTR [rbp-0x20]
  401f70:	89 45 e0             	mov    DWORD PTR [rbp-0x20],eax
  401f73:	e9 00 00 00 00       	jmp    401f78 <vsprintf+0x2b8>
  401f78:	e9 00 00 00 00       	jmp    401f7d <vsprintf+0x2bd>
				}
			}
  401f7d:	e9 db 03 00 00       	jmp    40235d <vsprintf+0x69d>
			else if (specifier == 'x')
  401f82:	83 7d cc 78          	cmp    DWORD PTR [rbp-0x34],0x78
  401f86:	0f 85 46 02 00 00    	jne    4021d2 <vsprintf+0x512>
			{
				if (length == WidthFlags_None)
  401f8c:	83 7d d8 00          	cmp    DWORD PTR [rbp-0x28],0x0
  401f90:	0f 85 b4 00 00 00    	jne    40204a <vsprintf+0x38a>
				{
					unsigned int number = va_arg(arg, unsigned int);
  401f96:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  401f9a:	8b 08                	mov    ecx,DWORD PTR [rax]
  401f9c:	83 f9 28             	cmp    ecx,0x28
  401f9f:	48 89 85 20 ff ff ff 	mov    QWORD PTR [rbp-0xe0],rax
  401fa6:	89 8d 1c ff ff ff    	mov    DWORD PTR [rbp-0xe4],ecx
  401fac:	0f 87 25 00 00 00    	ja     401fd7 <vsprintf+0x317>
  401fb2:	8b 85 1c ff ff ff    	mov    eax,DWORD PTR [rbp-0xe4]
  401fb8:	48 63 c8             	movsxd rcx,eax
  401fbb:	48 8b 95 20 ff ff ff 	mov    rdx,QWORD PTR [rbp-0xe0]
  401fc2:	48 03 4a 10          	add    rcx,QWORD PTR [rdx+0x10]
  401fc6:	83 c0 08             	add    eax,0x8
  401fc9:	89 02                	mov    DWORD PTR [rdx],eax
  401fcb:	48 89 8d 10 ff ff ff 	mov    QWORD PTR [rbp-0xf0],rcx
  401fd2:	e9 1d 00 00 00       	jmp    401ff4 <vsprintf+0x334>
  401fd7:	48 8b 85 20 ff ff ff 	mov    rax,QWORD PTR [rbp-0xe0]
  401fde:	48 8b 48 08          	mov    rcx,QWORD PTR [rax+0x8]
  401fe2:	48 89 ca             	mov    rdx,rcx
  401fe5:	48 83 c1 08          	add    rcx,0x8
  401fe9:	48 89 48 08          	mov    QWORD PTR [rax+0x8],rcx
  401fed:	48 89 95 10 ff ff ff 	mov    QWORD PTR [rbp-0xf0],rdx
  401ff4:	48 8b 85 10 ff ff ff 	mov    rax,QWORD PTR [rbp-0xf0]
  401ffb:	8b 08                	mov    ecx,DWORD PTR [rax]
  401ffd:	89 4d b4             	mov    DWORD PTR [rbp-0x4c],ecx
					buffer_index += print_number(number, 16, buffer, flags, width, precision, length, specifier);
  402000:	8b 4d b4             	mov    ecx,DWORD PTR [rbp-0x4c]
  402003:	89 cf                	mov    edi,ecx
  402005:	48 8b 55 f8          	mov    rdx,QWORD PTR [rbp-0x8]
  402009:	8b 4d dc             	mov    ecx,DWORD PTR [rbp-0x24]
  40200c:	44 8b 45 d4          	mov    r8d,DWORD PTR [rbp-0x2c]
  402010:	44 8b 4d d0          	mov    r9d,DWORD PTR [rbp-0x30]
  402014:	8b 75 d8             	mov    esi,DWORD PTR [rbp-0x28]
  402017:	44 8b 55 cc          	mov    r10d,DWORD PTR [rbp-0x34]
  40201b:	41 bb 10 00 00 00    	mov    r11d,0x10
  402021:	89 b5 0c ff ff ff    	mov    DWORD PTR [rbp-0xf4],esi
  402027:	44 89 de             	mov    esi,r11d
  40202a:	44 8b 9d 0c ff ff ff 	mov    r11d,DWORD PTR [rbp-0xf4]
  402031:	44 89 1c 24          	mov    DWORD PTR [rsp],r11d
  402035:	44 89 54 24 08       	mov    DWORD PTR [rsp+0x8],r10d
  40203a:	e8 a1 f7 ff ff       	call   4017e0 <print_number>
  40203f:	03 45 e0             	add    eax,DWORD PTR [rbp-0x20]
  402042:	89 45 e0             	mov    DWORD PTR [rbp-0x20],eax
				}
  402045:	e9 83 01 00 00       	jmp    4021cd <vsprintf+0x50d>
				else if (length == WidthFlags_l)
  40204a:	83 7d d8 03          	cmp    DWORD PTR [rbp-0x28],0x3
  40204e:	0f 85 b5 00 00 00    	jne    402109 <vsprintf+0x449>
				{
					unsigned long int number = va_arg(arg, unsigned long int);
  402054:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  402058:	8b 08                	mov    ecx,DWORD PTR [rax]
  40205a:	83 f9 28             	cmp    ecx,0x28
  40205d:	48 89 85 00 ff ff ff 	mov    QWORD PTR [rbp-0x100],rax
  402064:	89 8d fc fe ff ff    	mov    DWORD PTR [rbp-0x104],ecx
  40206a:	0f 87 25 00 00 00    	ja     402095 <vsprintf+0x3d5>
  402070:	8b 85 fc fe ff ff    	mov    eax,DWORD PTR [rbp-0x104]
  402076:	48 63 c8             	movsxd rcx,eax
  402079:	48 8b 95 00 ff ff ff 	mov    rdx,QWORD PTR [rbp-0x100]
  402080:	48 03 4a 10          	add    rcx,QWORD PTR [rdx+0x10]
  402084:	83 c0 08             	add    eax,0x8
  402087:	89 02                	mov    DWORD PTR [rdx],eax
  402089:	48 89 8d f0 fe ff ff 	mov    QWORD PTR [rbp-0x110],rcx
  402090:	e9 1d 00 00 00       	jmp    4020b2 <vsprintf+0x3f2>
  402095:	48 8b 85 00 ff ff ff 	mov    rax,QWORD PTR [rbp-0x100]
  40209c:	48 8b 48 08          	mov    rcx,QWORD PTR [rax+0x8]
  4020a0:	48 89 ca             	mov    rdx,rcx
  4020a3:	48 83 c1 08          	add    rcx,0x8
  4020a7:	48 89 48 08          	mov    QWORD PTR [rax+0x8],rcx
  4020ab:	48 89 95 f0 fe ff ff 	mov    QWORD PTR [rbp-0x110],rdx
  4020b2:	48 8b 85 f0 fe ff ff 	mov    rax,QWORD PTR [rbp-0x110]
  4020b9:	48 8b 00             	mov    rax,QWORD PTR [rax]
  4020bc:	48 89 45 a8          	mov    QWORD PTR [rbp-0x58],rax
					buffer_index += print_number(number, 16, buffer, flags, width, precision, length, specifier);
  4020c0:	48 8b 7d a8          	mov    rdi,QWORD PTR [rbp-0x58]
  4020c4:	48 8b 55 f8          	mov    rdx,QWORD PTR [rbp-0x8]
  4020c8:	8b 4d dc             	mov    ecx,DWORD PTR [rbp-0x24]
  4020cb:	44 8b 45 d4          	mov    r8d,DWORD PTR [rbp-0x2c]
  4020cf:	44 8b 4d d0          	mov    r9d,DWORD PTR [rbp-0x30]
  4020d3:	8b 75 d8             	mov    esi,DWORD PTR [rbp-0x28]
  4020d6:	44 8b 55 cc          	mov    r10d,DWORD PTR [rbp-0x34]
  4020da:	41 bb 10 00 00 00    	mov    r11d,0x10
  4020e0:	89 b5 ec fe ff ff    	mov    DWORD PTR [rbp-0x114],esi
  4020e6:	44 89 de             	mov    esi,r11d
  4020e9:	44 8b 9d ec fe ff ff 	mov    r11d,DWORD PTR [rbp-0x114]
  4020f0:	44 89 1c 24          	mov    DWORD PTR [rsp],r11d
  4020f4:	44 89 54 24 08       	mov    DWORD PTR [rsp+0x8],r10d
  4020f9:	e8 e2 f6 ff ff       	call   4017e0 <print_number>
  4020fe:	03 45 e0             	add    eax,DWORD PTR [rbp-0x20]
  402101:	89 45 e0             	mov    DWORD PTR [rbp-0x20],eax
				}
  402104:	e9 bf 00 00 00       	jmp    4021c8 <vsprintf+0x508>
				else if (length == WidthFlags_ll)
  402109:	83 7d d8 04          	cmp    DWORD PTR [rbp-0x28],0x4
  40210d:	0f 85 b0 00 00 00    	jne    4021c3 <vsprintf+0x503>
				{
					unsigned long long int number = va_arg(arg, unsigned long long int);
  402113:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  402117:	8b 08                	mov    ecx,DWORD PTR [rax]
  402119:	83 f9 28             	cmp    ecx,0x28
  40211c:	48 89 85 e0 fe ff ff 	mov    QWORD PTR [rbp-0x120],rax
  402123:	89 8d dc fe ff ff    	mov    DWORD PTR [rbp-0x124],ecx
  402129:	0f 87 25 00 00 00    	ja     402154 <vsprintf+0x494>
  40212f:	8b 85 dc fe ff ff    	mov    eax,DWORD PTR [rbp-0x124]
  402135:	48 63 c8             	movsxd rcx,eax
  402138:	48 8b 95 e0 fe ff ff 	mov    rdx,QWORD PTR [rbp-0x120]
  40213f:	48 03 4a 10          	add    rcx,QWORD PTR [rdx+0x10]
  402143:	83 c0 08             	add    eax,0x8
  402146:	89 02                	mov    DWORD PTR [rdx],eax
  402148:	48 89 8d d0 fe ff ff 	mov    QWORD PTR [rbp-0x130],rcx
  40214f:	e9 1d 00 00 00       	jmp    402171 <vsprintf+0x4b1>
  402154:	48 8b 85 e0 fe ff ff 	mov    rax,QWORD PTR [rbp-0x120]
  40215b:	48 8b 48 08          	mov    rcx,QWORD PTR [rax+0x8]
  40215f:	48 89 ca             	mov    rdx,rcx
  402162:	48 83 c1 08          	add    rcx,0x8
  402166:	48 89 48 08          	mov    QWORD PTR [rax+0x8],rcx
  40216a:	48 89 95 d0 fe ff ff 	mov    QWORD PTR [rbp-0x130],rdx
  402171:	48 8b 85 d0 fe ff ff 	mov    rax,QWORD PTR [rbp-0x130]
  402178:	48 8b 00             	mov    rax,QWORD PTR [rax]
  40217b:	48 89 45 a0          	mov    QWORD PTR [rbp-0x60],rax
					buffer_index += print_number(number, 16, buffer, flags, width, precision, length, specifier);
  40217f:	48 8b 7d a0          	mov    rdi,QWORD PTR [rbp-0x60]
  402183:	48 8b 55 f8          	mov    rdx,QWORD PTR [rbp-0x8]
  402187:	8b 4d dc             	mov    ecx,DWORD PTR [rbp-0x24]
  40218a:	44 8b 45 d4          	mov    r8d,DWORD PTR [rbp-0x2c]
  40218e:	44 8b 4d d0          	mov    r9d,DWORD PTR [rbp-0x30]
  402192:	8b 75 d8             	mov    esi,DWORD PTR [rbp-0x28]
  402195:	44 8b 55 cc          	mov    r10d,DWORD PTR [rbp-0x34]
  402199:	41 bb 10 00 00 00    	mov    r11d,0x10
  40219f:	89 b5 cc fe ff ff    	mov    DWORD PTR [rbp-0x134],esi
  4021a5:	44 89 de             	mov    esi,r11d
  4021a8:	44 8b 9d cc fe ff ff 	mov    r11d,DWORD PTR [rbp-0x134]
  4021af:	44 89 1c 24          	mov    DWORD PTR [rsp],r11d
  4021b3:	44 89 54 24 08       	mov    DWORD PTR [rsp+0x8],r10d
  4021b8:	e8 23 f6 ff ff       	call   4017e0 <print_number>
  4021bd:	03 45 e0             	add    eax,DWORD PTR [rbp-0x20]
  4021c0:	89 45 e0             	mov    DWORD PTR [rbp-0x20],eax
  4021c3:	e9 00 00 00 00       	jmp    4021c8 <vsprintf+0x508>
  4021c8:	e9 00 00 00 00       	jmp    4021cd <vsprintf+0x50d>
				}
			}
  4021cd:	e9 86 01 00 00       	jmp    402358 <vsprintf+0x698>
			else if (specifier == 's')
  4021d2:	83 7d cc 73          	cmp    DWORD PTR [rbp-0x34],0x73
  4021d6:	0f 85 72 01 00 00    	jne    40234e <vsprintf+0x68e>
			{
				char *str = va_arg(arg, char *);
  4021dc:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  4021e0:	8b 08                	mov    ecx,DWORD PTR [rax]
  4021e2:	83 f9 28             	cmp    ecx,0x28
  4021e5:	48 89 85 c0 fe ff ff 	mov    QWORD PTR [rbp-0x140],rax
  4021ec:	89 8d bc fe ff ff    	mov    DWORD PTR [rbp-0x144],ecx
  4021f2:	0f 87 25 00 00 00    	ja     40221d <vsprintf+0x55d>
  4021f8:	8b 85 bc fe ff ff    	mov    eax,DWORD PTR [rbp-0x144]
  4021fe:	48 63 c8             	movsxd rcx,eax
  402201:	48 8b 95 c0 fe ff ff 	mov    rdx,QWORD PTR [rbp-0x140]
  402208:	48 03 4a 10          	add    rcx,QWORD PTR [rdx+0x10]
  40220c:	83 c0 08             	add    eax,0x8
  40220f:	89 02                	mov    DWORD PTR [rdx],eax
  402211:	48 89 8d b0 fe ff ff 	mov    QWORD PTR [rbp-0x150],rcx
  402218:	e9 1d 00 00 00       	jmp    40223a <vsprintf+0x57a>
  40221d:	48 8b 85 c0 fe ff ff 	mov    rax,QWORD PTR [rbp-0x140]
  402224:	48 8b 48 08          	mov    rcx,QWORD PTR [rax+0x8]
  402228:	48 89 ca             	mov    rdx,rcx
  40222b:	48 83 c1 08          	add    rcx,0x8
  40222f:	48 89 48 08          	mov    QWORD PTR [rax+0x8],rcx
  402233:	48 89 95 b0 fe ff ff 	mov    QWORD PTR [rbp-0x150],rdx
  40223a:	48 8b 85 b0 fe ff ff 	mov    rax,QWORD PTR [rbp-0x150]
  402241:	48 8b 00             	mov    rax,QWORD PTR [rax]
  402244:	48 89 45 98          	mov    QWORD PTR [rbp-0x68],rax
				int ch_len = 0;
  402248:	c7 45 94 00 00 00 00 	mov    DWORD PTR [rbp-0x6c],0x0
				if (precision == -1)
  40224f:	83 7d d0 ff          	cmp    DWORD PTR [rbp-0x30],0xffffffff
  402253:	0f 85 13 00 00 00    	jne    40226c <vsprintf+0x5ac>
					ch_len = strlen(str);
  402259:	48 8b 7d 98          	mov    rdi,QWORD PTR [rbp-0x68]
  40225d:	e8 fe df ff ff       	call   400260 <strlen>
  402262:	89 c1                	mov    ecx,eax
  402264:	89 4d 94             	mov    DWORD PTR [rbp-0x6c],ecx
  402267:	e9 60 00 00 00       	jmp    4022cc <vsprintf+0x60c>
				else
					for (; ch_len < precision && str[ch_len] != 0; ch_len++)
  40226c:	e9 00 00 00 00       	jmp    402271 <vsprintf+0x5b1>
  402271:	31 c0                	xor    eax,eax
  402273:	88 c1                	mov    cl,al
  402275:	8b 45 94             	mov    eax,DWORD PTR [rbp-0x6c]
  402278:	3b 45 d0             	cmp    eax,DWORD PTR [rbp-0x30]
  40227b:	88 8d af fe ff ff    	mov    BYTE PTR [rbp-0x151],cl
  402281:	0f 8d 1a 00 00 00    	jge    4022a1 <vsprintf+0x5e1>
  402287:	48 8b 45 98          	mov    rax,QWORD PTR [rbp-0x68]
  40228b:	48 63 4d 94          	movsxd rcx,DWORD PTR [rbp-0x6c]
  40228f:	0f be 14 08          	movsx  edx,BYTE PTR [rax+rcx*1]
  402293:	83 fa 00             	cmp    edx,0x0
  402296:	40 0f 95 c6          	setne  sil
  40229a:	40 88 b5 af fe ff ff 	mov    BYTE PTR [rbp-0x151],sil
  4022a1:	8a 85 af fe ff ff    	mov    al,BYTE PTR [rbp-0x151]
  4022a7:	a8 01                	test   al,0x1
  4022a9:	0f 85 05 00 00 00    	jne    4022b4 <vsprintf+0x5f4>
  4022af:	e9 13 00 00 00       	jmp    4022c7 <vsprintf+0x607>
  4022b4:	e9 00 00 00 00       	jmp    4022b9 <vsprintf+0x5f9>
  4022b9:	8b 45 94             	mov    eax,DWORD PTR [rbp-0x6c]
  4022bc:	83 c0 01             	add    eax,0x1
  4022bf:	89 45 94             	mov    DWORD PTR [rbp-0x6c],eax
  4022c2:	e9 aa ff ff ff       	jmp    402271 <vsprintf+0x5b1>
  4022c7:	e9 00 00 00 00       	jmp    4022cc <vsprintf+0x60c>
						;
				for (int i = 0; i < width - ch_len; i++)
  4022cc:	c7 45 90 00 00 00 00 	mov    DWORD PTR [rbp-0x70],0x0
  4022d3:	8b 45 90             	mov    eax,DWORD PTR [rbp-0x70]
  4022d6:	8b 4d d4             	mov    ecx,DWORD PTR [rbp-0x2c]
  4022d9:	2b 4d 94             	sub    ecx,DWORD PTR [rbp-0x6c]
  4022dc:	39 c8                	cmp    eax,ecx
  4022de:	0f 8d 24 00 00 00    	jge    402308 <vsprintf+0x648>
				{
					buffer[buffer_index++] = ' ';
  4022e4:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4022e8:	8b 4d e0             	mov    ecx,DWORD PTR [rbp-0x20]
  4022eb:	89 ca                	mov    edx,ecx
  4022ed:	83 c2 01             	add    edx,0x1
  4022f0:	89 55 e0             	mov    DWORD PTR [rbp-0x20],edx
  4022f3:	48 63 f1             	movsxd rsi,ecx
  4022f6:	c6 04 30 20          	mov    BYTE PTR [rax+rsi*1],0x20
				for (int i = 0; i < width - ch_len; i++)
  4022fa:	8b 45 90             	mov    eax,DWORD PTR [rbp-0x70]
  4022fd:	83 c0 01             	add    eax,0x1
  402300:	89 45 90             	mov    DWORD PTR [rbp-0x70],eax
  402303:	e9 cb ff ff ff       	jmp    4022d3 <vsprintf+0x613>
				}
				for (int i = 0; i < ch_len; i++)
  402308:	c7 45 8c 00 00 00 00 	mov    DWORD PTR [rbp-0x74],0x0
  40230f:	8b 45 8c             	mov    eax,DWORD PTR [rbp-0x74]
  402312:	3b 45 94             	cmp    eax,DWORD PTR [rbp-0x6c]
  402315:	0f 8d 2e 00 00 00    	jge    402349 <vsprintf+0x689>
				{
					buffer[buffer_index++] = str[i];
  40231b:	48 8b 45 98          	mov    rax,QWORD PTR [rbp-0x68]
  40231f:	48 63 4d 8c          	movsxd rcx,DWORD PTR [rbp-0x74]
  402323:	8a 14 08             	mov    dl,BYTE PTR [rax+rcx*1]
  402326:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  40232a:	8b 75 e0             	mov    esi,DWORD PTR [rbp-0x20]
  40232d:	89 f7                	mov    edi,esi
  40232f:	83 c7 01             	add    edi,0x1
  402332:	89 7d e0             	mov    DWORD PTR [rbp-0x20],edi
  402335:	48 63 ce             	movsxd rcx,esi
  402338:	88 14 08             	mov    BYTE PTR [rax+rcx*1],dl
				for (int i = 0; i < ch_len; i++)
  40233b:	8b 45 8c             	mov    eax,DWORD PTR [rbp-0x74]
  40233e:	83 c0 01             	add    eax,0x1
  402341:	89 45 8c             	mov    DWORD PTR [rbp-0x74],eax
  402344:	e9 c6 ff ff ff       	jmp    40230f <vsprintf+0x64f>
				}
			}
  402349:	e9 05 00 00 00       	jmp    402353 <vsprintf+0x693>
			else
				goto safe_exit;
  40234e:	e9 41 00 00 00       	jmp    402394 <vsprintf+0x6d4>
  402353:	e9 00 00 00 00       	jmp    402358 <vsprintf+0x698>
  402358:	e9 00 00 00 00       	jmp    40235d <vsprintf+0x69d>
		}
  40235d:	e9 28 00 00 00       	jmp    40238a <vsprintf+0x6ca>
		else
		{
			buffer[buffer_index] = format[format_index];
  402362:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  402366:	48 63 4d e4          	movsxd rcx,DWORD PTR [rbp-0x1c]
  40236a:	8a 14 08             	mov    dl,BYTE PTR [rax+rcx*1]
  40236d:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  402371:	48 63 4d e0          	movsxd rcx,DWORD PTR [rbp-0x20]
  402375:	88 14 08             	mov    BYTE PTR [rax+rcx*1],dl
			buffer_index++;
  402378:	8b 75 e0             	mov    esi,DWORD PTR [rbp-0x20]
  40237b:	83 c6 01             	add    esi,0x1
  40237e:	89 75 e0             	mov    DWORD PTR [rbp-0x20],esi
			format_index++;
  402381:	8b 75 e4             	mov    esi,DWORD PTR [rbp-0x1c]
  402384:	83 c6 01             	add    esi,0x1
  402387:	89 75 e4             	mov    DWORD PTR [rbp-0x1c],esi
	while (format[format_index] != 0)
  40238a:	e9 56 f9 ff ff       	jmp    401ce5 <vsprintf+0x25>
  40238f:	e9 00 00 00 00       	jmp    402394 <vsprintf+0x6d4>
		}
	}
safe_exit:
	buffer[buffer_index] = 0;
  402394:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  402398:	48 63 4d e0          	movsxd rcx,DWORD PTR [rbp-0x20]
  40239c:	c6 04 08 00          	mov    BYTE PTR [rax+rcx*1],0x0
	return buffer_index;
  4023a0:	8b 45 e0             	mov    eax,DWORD PTR [rbp-0x20]
  4023a3:	48 81 c4 70 01 00 00 	add    rsp,0x170
  4023aa:	5d                   	pop    rbp
  4023ab:	c3                   	ret    
  4023ac:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]

00000000004023b0 <_start>:
 */
extern void exit(int status);
extern int main();

void _start()
{
  4023b0:	55                   	push   rbp
  4023b1:	48 89 e5             	mov    rbp,rsp
    exit(main());
  4023b4:	b0 00                	mov    al,0x0
  4023b6:	e8 35 dd ff ff       	call   4000f0 <main>
  4023bb:	89 c7                	mov    edi,eax
  4023bd:	e8 4e 00 00 00       	call   402410 <exit>
  4023c2:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
  4023c9:	00 00 00 
  4023cc:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]

00000000004023d0 <_malloc>:
extern void *_malloc(uint32_t length);
extern void _free(void *ptr);
extern void *_aligned_malloc(uint32_t len, int n);
#else
void *_malloc(uint32_t length)
{
  4023d0:	55                   	push   rbp
  4023d1:	48 89 e5             	mov    rbp,rsp
  4023d4:	89 7d fc             	mov    DWORD PTR [rbp-0x4],edi
    __asm__("int3");
  4023d7:	cc                   	int3   
  4023d8:	31 ff                	xor    edi,edi
  4023da:	89 f8                	mov    eax,edi
    return NULL;
  4023dc:	5d                   	pop    rbp
  4023dd:	c3                   	ret    
  4023de:	66 90                	xchg   ax,ax

00000000004023e0 <_free>:
}
void _free(void *ptr) { __asm__("int3"); }
  4023e0:	55                   	push   rbp
  4023e1:	48 89 e5             	mov    rbp,rsp
  4023e4:	48 89 7d f8          	mov    QWORD PTR [rbp-0x8],rdi
  4023e8:	cc                   	int3   
  4023e9:	5d                   	pop    rbp
  4023ea:	c3                   	ret    
  4023eb:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]

00000000004023f0 <_aligned_malloc>:
void *_aligned_malloc(uint32_t len, int n)
{
  4023f0:	55                   	push   rbp
  4023f1:	48 89 e5             	mov    rbp,rsp
  4023f4:	89 7d fc             	mov    DWORD PTR [rbp-0x4],edi
  4023f7:	89 75 f8             	mov    DWORD PTR [rbp-0x8],esi
    __asm__("int3");
  4023fa:	cc                   	int3   
  4023fb:	31 f6                	xor    esi,esi
  4023fd:	89 f0                	mov    eax,esi
    return NULL;
  4023ff:	5d                   	pop    rbp
  402400:	c3                   	ret    
  402401:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
  402408:	00 00 00 
  40240b:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]

0000000000402410 <exit>:
}
__attribute__((noreturn)) void exit(int status)
{
  402410:	55                   	push   rbp
  402411:	48 89 e5             	mov    rbp,rsp
  402414:	89 7d fc             	mov    DWORD PTR [rbp-0x4],edi
    __asm__ volatile("syscall" ::"D"(SYSCALL_EXIT), "S"(status)
  402417:	8b 75 fc             	mov    esi,DWORD PTR [rbp-0x4]
  40241a:	bf 02 00 00 00       	mov    edi,0x2
  40241f:	0f 05                	syscall 
								:"rcx","r11");
    while (1)
    {
        __asm__("pause");
  402421:	f3 90                	pause  
    while (1)
  402423:	e9 f9 ff ff ff       	jmp    402421 <exit+0x11>
  402428:	0f 1f 84 00 00 00 00 	nop    DWORD PTR [rax+rax*1+0x0]
  40242f:	00 

0000000000402430 <aligned_malloc>:
}
#endif
size_t mem_used = 0;

void *aligned_malloc(size_t size, int n)
{
  402430:	55                   	push   rbp
  402431:	48 89 e5             	mov    rbp,rsp
  402434:	48 83 ec 30          	sub    rsp,0x30
  402438:	48 89 7d f0          	mov    QWORD PTR [rbp-0x10],rdi
  40243c:	89 75 ec             	mov    DWORD PTR [rbp-0x14],esi
    volatile void *ptr = _aligned_malloc(size, n);
  40243f:	48 8b 7d f0          	mov    rdi,QWORD PTR [rbp-0x10]
  402443:	89 fe                	mov    esi,edi
  402445:	8b 45 ec             	mov    eax,DWORD PTR [rbp-0x14]
  402448:	89 f7                	mov    edi,esi
  40244a:	89 c6                	mov    esi,eax
  40244c:	e8 9f ff ff ff       	call   4023f0 <_aligned_malloc>
  402451:	48 89 45 e0          	mov    QWORD PTR [rbp-0x20],rax
    if (!ptr)
  402455:	48 83 7d e0 00       	cmp    QWORD PTR [rbp-0x20],0x0
  40245a:	0f 85 23 00 00 00    	jne    402483 <aligned_malloc+0x53>
    {
        printf("\nmm error.");
  402460:	48 bf 50 27 40 00 00 	movabs rdi,0x402750
  402467:	00 00 00 
  40246a:	b0 00                	mov    al,0x0
  40246c:	e8 1f e7 ff ff       	call   400b90 <printf>
        __asm__("cli;hlt");
  402471:	fa                   	cli    
  402472:	f4                   	hlt    
        return NULL;
  402473:	48 c7 45 f8 00 00 00 	mov    QWORD PTR [rbp-0x8],0x0
  40247a:	00 
  40247b:	89 45 dc             	mov    DWORD PTR [rbp-0x24],eax
  40247e:	e9 2e 00 00 00       	jmp    4024b1 <aligned_malloc+0x81>
    }
    mem_used += size;
  402483:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  402487:	48 03 04 25 08 30 60 	add    rax,QWORD PTR ds:0x603008
  40248e:	00 
  40248f:	48 89 04 25 08 30 60 	mov    QWORD PTR ds:0x603008,rax
  402496:	00 
    memset((void *)ptr, 0xcc, size);
  402497:	48 8b 7d e0          	mov    rdi,QWORD PTR [rbp-0x20]
  40249b:	48 8b 55 f0          	mov    rdx,QWORD PTR [rbp-0x10]
  40249f:	be cc 00 00 00       	mov    esi,0xcc
  4024a4:	e8 a7 dc ff ff       	call   400150 <memset>
    return (void *)ptr;
  4024a9:	48 8b 45 e0          	mov    rax,QWORD PTR [rbp-0x20]
  4024ad:	48 89 45 f8          	mov    QWORD PTR [rbp-0x8],rax
}
  4024b1:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  4024b5:	48 83 c4 30          	add    rsp,0x30
  4024b9:	5d                   	pop    rbp
  4024ba:	c3                   	ret    
  4024bb:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]

00000000004024c0 <malloc>:

void *malloc(size_t size)
{
  4024c0:	55                   	push   rbp
  4024c1:	48 89 e5             	mov    rbp,rsp
  4024c4:	48 83 ec 20          	sub    rsp,0x20
  4024c8:	48 89 7d f0          	mov    QWORD PTR [rbp-0x10],rdi
    volatile void *ptr = _malloc(size);
  4024cc:	48 8b 7d f0          	mov    rdi,QWORD PTR [rbp-0x10]
  4024d0:	89 f8                	mov    eax,edi
  4024d2:	89 c7                	mov    edi,eax
  4024d4:	e8 f7 fe ff ff       	call   4023d0 <_malloc>
  4024d9:	48 89 45 e8          	mov    QWORD PTR [rbp-0x18],rax
    if (!ptr)
  4024dd:	48 83 7d e8 00       	cmp    QWORD PTR [rbp-0x18],0x0
  4024e2:	0f 85 23 00 00 00    	jne    40250b <malloc+0x4b>
    {
        printf("\nmm error.");
  4024e8:	48 bf 50 27 40 00 00 	movabs rdi,0x402750
  4024ef:	00 00 00 
  4024f2:	b0 00                	mov    al,0x0
  4024f4:	e8 97 e6 ff ff       	call   400b90 <printf>
        __asm__("cli;hlt");
  4024f9:	fa                   	cli    
  4024fa:	f4                   	hlt    
        return NULL;
  4024fb:	48 c7 45 f8 00 00 00 	mov    QWORD PTR [rbp-0x8],0x0
  402502:	00 
  402503:	89 45 e4             	mov    DWORD PTR [rbp-0x1c],eax
  402506:	e9 2e 00 00 00       	jmp    402539 <malloc+0x79>
    }
    mem_used += size;
  40250b:	48 8b 45 f0          	mov    rax,QWORD PTR [rbp-0x10]
  40250f:	48 03 04 25 08 30 60 	add    rax,QWORD PTR ds:0x603008
  402516:	00 
  402517:	48 89 04 25 08 30 60 	mov    QWORD PTR ds:0x603008,rax
  40251e:	00 
    memset((void *)ptr, 0xcc, size);
  40251f:	48 8b 7d e8          	mov    rdi,QWORD PTR [rbp-0x18]
  402523:	48 8b 55 f0          	mov    rdx,QWORD PTR [rbp-0x10]
  402527:	be cc 00 00 00       	mov    esi,0xcc
  40252c:	e8 1f dc ff ff       	call   400150 <memset>
    return (void *)ptr;
  402531:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  402535:	48 89 45 f8          	mov    QWORD PTR [rbp-0x8],rax
}
  402539:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  40253d:	48 83 c4 20          	add    rsp,0x20
  402541:	5d                   	pop    rbp
  402542:	c3                   	ret    
  402543:	66 2e 0f 1f 84 00 00 	nop    WORD PTR cs:[rax+rax*1+0x0]
  40254a:	00 00 00 
  40254d:	0f 1f 00             	nop    DWORD PTR [rax]

0000000000402550 <free>:

void free(void *ptr)
{
  402550:	55                   	push   rbp
  402551:	48 89 e5             	mov    rbp,rsp
  402554:	48 83 ec 10          	sub    rsp,0x10
  402558:	48 89 7d f8          	mov    QWORD PTR [rbp-0x8],rdi
    _free(ptr);
  40255c:	48 8b 7d f8          	mov    rdi,QWORD PTR [rbp-0x8]
  402560:	e8 7b fe ff ff       	call   4023e0 <_free>
}
  402565:	48 83 c4 10          	add    rsp,0x10
  402569:	5d                   	pop    rbp
  40256a:	c3                   	ret    
  40256b:	0f 1f 44 00 00       	nop    DWORD PTR [rax+rax*1+0x0]

0000000000402570 <realloc>:

void *realloc(void *ptr, size_t size)
{
  402570:	55                   	push   rbp
  402571:	48 89 e5             	mov    rbp,rsp
  402574:	48 83 ec 10          	sub    rsp,0x10
  402578:	48 89 7d f8          	mov    QWORD PTR [rbp-0x8],rdi
  40257c:	48 89 75 f0          	mov    QWORD PTR [rbp-0x10],rsi
    _free(ptr);
  402580:	48 8b 7d f8          	mov    rdi,QWORD PTR [rbp-0x8]
  402584:	e8 57 fe ff ff       	call   4023e0 <_free>
    return _malloc(size);
  402589:	48 8b 75 f0          	mov    rsi,QWORD PTR [rbp-0x10]
  40258d:	89 f0                	mov    eax,esi
  40258f:	89 c7                	mov    edi,eax
  402591:	e8 3a fe ff ff       	call   4023d0 <_malloc>
  402596:	48 83 c4 10          	add    rsp,0x10
  40259a:	5d                   	pop    rbp
  40259b:	c3                   	ret    
  40259c:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]

00000000004025a0 <calloc>:
}

void *calloc(size_t blocks, size_t size)
{
  4025a0:	55                   	push   rbp
  4025a1:	48 89 e5             	mov    rbp,rsp
  4025a4:	48 83 ec 30          	sub    rsp,0x30
  4025a8:	48 89 7d f0          	mov    QWORD PTR [rbp-0x10],rdi
  4025ac:	48 89 75 e8          	mov    QWORD PTR [rbp-0x18],rsi
    void *ptr = _malloc(size * blocks);
  4025b0:	48 8b 75 e8          	mov    rsi,QWORD PTR [rbp-0x18]
  4025b4:	48 0f af 75 f0       	imul   rsi,QWORD PTR [rbp-0x10]
  4025b9:	89 f0                	mov    eax,esi
  4025bb:	89 c7                	mov    edi,eax
  4025bd:	e8 0e fe ff ff       	call   4023d0 <_malloc>
  4025c2:	48 89 45 e0          	mov    QWORD PTR [rbp-0x20],rax
    if (!ptr)
  4025c6:	48 83 7d e0 00       	cmp    QWORD PTR [rbp-0x20],0x0
  4025cb:	0f 85 23 00 00 00    	jne    4025f4 <calloc+0x54>
    {
        printf("\nmm error.");
  4025d1:	48 bf 50 27 40 00 00 	movabs rdi,0x402750
  4025d8:	00 00 00 
  4025db:	b0 00                	mov    al,0x0
  4025dd:	e8 ae e5 ff ff       	call   400b90 <printf>
        __asm__("cli;hlt");
  4025e2:	fa                   	cli    
  4025e3:	f4                   	hlt    
        return NULL;
  4025e4:	48 c7 45 f8 00 00 00 	mov    QWORD PTR [rbp-0x8],0x0
  4025eb:	00 
  4025ec:	89 45 dc             	mov    DWORD PTR [rbp-0x24],eax
  4025ef:	e9 38 00 00 00       	jmp    40262c <calloc+0x8c>
  4025f4:	31 f6                	xor    esi,esi
    }
    memset(ptr, 0, size * blocks);
  4025f6:	48 8b 7d e0          	mov    rdi,QWORD PTR [rbp-0x20]
  4025fa:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  4025fe:	48 0f af 45 f0       	imul   rax,QWORD PTR [rbp-0x10]
  402603:	48 89 c2             	mov    rdx,rax
  402606:	e8 45 db ff ff       	call   400150 <memset>
    mem_used += (size * blocks);
  40260b:	48 8b 45 e8          	mov    rax,QWORD PTR [rbp-0x18]
  40260f:	48 0f af 45 f0       	imul   rax,QWORD PTR [rbp-0x10]
  402614:	48 03 04 25 08 30 60 	add    rax,QWORD PTR ds:0x603008
  40261b:	00 
  40261c:	48 89 04 25 08 30 60 	mov    QWORD PTR ds:0x603008,rax
  402623:	00 
    return ptr;
  402624:	48 8b 45 e0          	mov    rax,QWORD PTR [rbp-0x20]
  402628:	48 89 45 f8          	mov    QWORD PTR [rbp-0x8],rax
  40262c:	48 8b 45 f8          	mov    rax,QWORD PTR [rbp-0x8]
  402630:	48 83 c4 30          	add    rsp,0x30
  402634:	5d                   	pop    rbp
  402635:	c3                   	ret    
