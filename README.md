# SpartaDivers (3rd Person TPS Shooter Game)

이 프로젝트는 **3인칭 TPS(Third-Person Shooter)** 슈팅 게임으로, 언리얼 엔진을 사용하여 제작되었습니다. 게임은 플레이어가 3인칭 시점에서 총기를 사용하여 적을 물리치는 액션을 제공합니다.

## 프로젝트 개요

이 프로젝트는 플레이어가 생존, 섬멸, 점령, 보스전 총 4개의 미션을 클리어하면 최종적으로 게임이 완료되는 형태로 구현하였습니다. 게임에는 플레이어 캐릭터, 총기 시스템, 적 AI 및 다양한 게임 메커니즘이 포함되어 있습니다.

- **생존**: 적을 처치하며 일정 시간동안 생존합니다.
- **섬멸**: 소환되는 적을 모두 처치합니다.
- **점령**: 특정 지역을 점령하면서 적을 처치합니다.
- **보스전**: 최종적으로 등장하는 강력한 보스를 처치합니다.

## 필수 기능 구현 목록

- **캐릭터 이동 및 상태 변화**: 걷기, 달리기, 점프, 앉기, 구르기 모션을 구현하였습니다.
- **무기 발사 및 데미지 처리**: 게임 시작시 기본적으로 제공되는 AR 무기를 구현하였습니다.
- **적 AI**: 플레이어를 추적하고 공격할 수 있는 근,원거리 적 AI를 구현하였습니다.
- **게임 모드**: 적 처치시 점수를 획득하고 최종적으로 보스를 처치하면 게임이 끝나는 형태로 구현하였습니다.
- **UI 시스템**: 플레이어의 체력, 방어력, 소지 무기 및 탄약 개수 등 중요한 정보를 화면에 표시합니다.

## 도전 기능 구현 목록

- **보스전 시스템**: 다양한 공격 패턴을 가진 보스 AI를 구현하였습니다.
- **고급 무기 시스템**: 적 처치시 랜덤으로 부착물 및 각종 소비템을 획득할 수 있고 미션을 클리어 할때마다 새로운 총기를 지급하는 형태로 구현하였습니다.
- **인벤토리 시스템**: 획득한 각종 아이템 관리 및 무기, 소비 슬롯에 원하는 아이템을 장착하고 사용할 수 있도록 구현하였습니다.

## 기술 스택

- **엔진**: 언리얼 엔진 5.5.3
- **프로그래밍 언어**: C++
- **플랫폼**: PC (Windows)

## 게임 조작법

- **W,A,S,D**: 이동
- **C**: 누르는 동안 웅크리기
- **X**: 구르기
- **Spacebar**: 점프
- **좌 Shift**: 달리기
- **R**: 재장전
- **Tab**: 무기 교체
- **E**: 상호작용
- **I**: 인벤토리
- **1,2,3**: 아이템 사용
- **마우스 좌클릭**: 총알 발사
- **마우스 우클릭**: 누르는 동안 견착

## 개발 기간

- 2025.02.17 ~ 2025.03.07
