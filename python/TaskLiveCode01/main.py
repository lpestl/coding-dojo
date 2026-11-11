import asyncio
from abc import ABC, abstractmethod
from collections import deque

queue: deque = deque()
counter = 0
lock = asyncio.Lock()


class Request(ABC):
    @abstractmethod
    def action(self) -> None:
        ...


class SampleRequest(Request):
    def __init__(self, request_id: int):
        self.request_id = request_id

    def action(self) -> None:
        print(f"Request #{self.request_id} in progress...")


async def producer():
    global counter
    request_id = 0
    while True:
        await asyncio.sleep(2)
        queue.append(SampleRequest(request_id))
        async with lock:
            counter += 1
        print(f"producer: new request #{request_id} added, counter = {counter}")
        request_id += 1


async def resolve():
    async with lock:
        request = queue[0]
        request.action()
        await post_resolve()


async def post_resolve():
    global counter
    async with lock:
        counter -= 1
        request = queue.popleft()
        print(f"post_resolve: request #{request.request_id} resolved, counter = {counter}")


async def main():
    producer_task = asyncio.create_task(producer())

    await asyncio.sleep(2.2)

    try:
        await asyncio.wait_for(resolve(), timeout=3.0)
    except asyncio.TimeoutError:
        print("Freezing. Execution halt.")

    producer_task.cancel()
    try:
        await producer_task
    except asyncio.CancelledError:
        pass


if __name__ == "__main__":
    asyncio.run(main())
