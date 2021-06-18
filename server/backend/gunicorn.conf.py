from multiprocessing import cpu_count

bind = 'localhost:5001'
wsgi_app = 'wsgi:app'
workers = cpu_count() + 1
backlog = 2048
