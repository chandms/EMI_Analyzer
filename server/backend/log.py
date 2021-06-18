'''
Author: Thirawat Bureetes
Email: tbureete@purdue.edu
Date: 06/13/2021
Description: This module is from logging.
'''

import logging
import time

logger = logging
logger.basicConfig(format='%(asctime)s %(levelname)s - %(message)s', 
                   datefmt='%d-%b-%y %H:%M:%S %Z',
                   level=logging.INFO)
logger.Formatter.converter = time.gmtime

