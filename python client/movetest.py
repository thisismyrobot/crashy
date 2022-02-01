# Simple test of motion to ensure it's all working.
from spherov2 import scanner


def main():
    with scanner.find_toy() as toy:
        import pdb; pdb.set_trace()

if __name__ == '__main__':
    main()
