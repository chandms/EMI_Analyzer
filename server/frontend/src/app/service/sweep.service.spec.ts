import { TestBed } from '@angular/core/testing';

import { SweepService } from './sweep.service';

describe('SweepService', () => {
  let service: SweepService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(SweepService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
