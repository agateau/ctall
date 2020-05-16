#!/usr/bin/env python

"""The setup script."""

from setuptools import setup, find_packages

requirements = [ ]

setup_requirements = ['pytest-runner', ]

test_requirements = ['pytest>=3', ]

setup(
    author="Aurélien Gâteau",
    author_email='mail@agateau.com',
    python_requires='>=3.5',
    classifiers=[
        'Development Status :: 2 - Pre-Alpha',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: GNU General Public License v3 (GPLv3)',
        'Natural Language :: English',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
    ],
    description="Catch them all",
    entry_points={
        'gui_scripts': [
            'ctall=ctall.main:main',
        ],
    },
    install_requires=requirements,
    license="GNU General Public License v3",
    include_package_data=True,
    keywords='ctall',
    name='ctall',
    packages=find_packages(include=['ctall', 'ctall.*']),
    setup_requires=setup_requirements,
    test_suite='tests',
    tests_require=test_requirements,
    url='https://github.com/agateau/ctall',
    version='0.1.0',
    zip_safe=False,
)
